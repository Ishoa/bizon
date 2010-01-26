#include "MoteurAudio.h"

CMoteurAudio::CMoteurAudio()
{
	m_pMoteur = NULL;		
	m_pAudioDevice = NULL; 
	m_pAudioContext = NULL;		


}

CMoteurAudio::CMoteurAudio(CMoteur* pMoteur)
{
	Console << "Constructeur CMoteurAudio()" << endl;
	m_pMoteur = pMoteur;		
	m_pAudioDevice = NULL; 
	m_pAudioContext = NULL;	


}

CMoteurAudio::~CMoteurAudio()
{
	Console << "Destructeur CMoteurAudio()" << endl;

	for(int i=0; i<Max_SOUND;i++)
	{
		// Destruction du tampon
		alDeleteBuffers(1, &m_pBuffers[i]);

		// Destruction de la source
		alSourcei(m_pSources[i], AL_BUFFER, 0);
		alDeleteSources(1, &m_pSources[i]);
	}

	// D�sactivation du contexte
	alcMakeContextCurrent(NULL);

	// Destruction du contexte
	alcDestroyContext(m_pAudioContext);

	// Fermeture du device
	alcCloseDevice(m_pAudioDevice);

	SAFE_DELETE(m_pAudioContext);
	SAFE_DELETE(m_pAudioDevice);
}

bool CMoteurAudio::Creation(const char* DeviceName)
{
	// Ouverture du device
	m_pAudioDevice = alcOpenDevice(DeviceName);
	if (!m_pAudioDevice)
	{
		Console<<"erreur Initialisation device moteur audio"<<endl;
		return false;
	}

	// Cr�ation du contexte
	m_pAudioContext = alcCreateContext(m_pAudioDevice, NULL);
	if (!m_pAudioContext)
	{
		Console<<"erreur Initialisation Context moteur audio"<<endl;
		return false;
	}

	// Activation du contexte
	if (!alcMakeContextCurrent(m_pAudioContext))
	{
		Console<<"erreur activation context moteur audio"<<endl;
		return false;
	}

	return true;
}


/// R�cup�re la liste des noms des devices disponibles
/// \param Devices : Tableau de cha�nes � remplir avec les noms des devices
void CMoteurAudio::GetDevicesDisponible(std::vector<std::string>& Devices)
{
	// Vidage de la liste
	Devices.clear();

	// R�cup�ration des devices disponibles
	const ALCchar* DeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

	if (DeviceList)
	{
		// Extraction des devices contenus dans la cha�ne renvoy�e
		while (strlen(DeviceList) > 0)
		{
			Devices.push_back(DeviceList);
			DeviceList += strlen(DeviceList) + 1;
		}
	}
	else
	{
		Console<< "Impossible de r�cup�rer la liste des devices" <<endl;
	}
}


////////////////////////////////////////////////////////////
/// Cr�e un tampon OpenAL � partir d'un fichier audio
/// \param Filename : Nom du fichier audio � charger
/// \return Identificateur du tampon OpenAL (0 si �chec)

bool CMoteurAudio::LoadSound(const std::string& Filename , unsigned int indice)
{
	// Ouverture du fichier audio avec libsndfile
	SF_INFO FileInfos;
	SNDFILE* File = sf_open(Filename.c_str(), SFM_READ, &FileInfos);
	if (!File)
	{
		Console << "Impossible d'ouvrir le fichier audio " <<endl;
		return false;
	}

	// Lecture du nombre d'�chantillons et du taux d'�chantillonnage (nombre d'�chantillons � lire par seconde)
	ALsizei NbSamples  = static_cast<ALsizei>(FileInfos.channels * FileInfos.frames);
	ALsizei SampleRate = static_cast<ALsizei>(FileInfos.samplerate);

	// Lecture des �chantillons audio au format entier 16 bits sign� (le plus commun)
	std::vector<ALshort> Samples(NbSamples);
	if (sf_read_short(File, &Samples[0], NbSamples) < NbSamples)
	{
		Console << "Impossible de lire les �chantillons stock�s dans le fichier audio" <<endl;
		return false;
	}

	// Fermeture du fichier
	sf_close(File);

	// D�termination du format en fonction du nombre de canaux
	ALenum Format;
	switch (FileInfos.channels)
	{
	case 1 : Format = AL_FORMAT_MONO16;   break;
	case 2 : Format = AL_FORMAT_STEREO16; break;
	default :
		Console << "Format audio non support� (plus de 2 canaux)" <<endl;
		return false;
	}

	// Cr�ation du tampon OpenAL
	alGenBuffers(1, &m_pBuffers[indice]);

	// Remplissage avec les �chantillons lus
	alBufferData(m_pBuffers[indice], Format, &Samples[0], NbSamples * sizeof(ALushort), SampleRate);

	// V�rification des erreurs
	if (alGetError() != AL_NO_ERROR)
	{
		Console << "Impossible de remplir le tampon OpenAL avec les �chantillons du fichier audio" << endl;
		return false;
	}

	if (m_pBuffers[indice] == 0)
		return false;

	// Cr�ation d'une source
	alGenSources(1, &m_pSources[indice]);
	alSourcei(m_pSources[indice], AL_BUFFER, m_pBuffers[indice]);

	return true;
}



void CMoteurAudio::PlaySound(unsigned int bIndice, bool bForcer)
{
	if(bIndice>=Max_SOUND)
		return;

	ALint Status;
	ALfloat Seconds = 0.f;
	alGetSourcef(m_pSources[bIndice], AL_SEC_OFFSET, &Seconds);
	//Console<< "\rLecture en cours... "<< Seconds << " sec";

	// R�cup�ration de l'�tat du son
	alGetSourcei(m_pSources[bIndice], AL_SOURCE_STATE, &Status);

	if(Status != AL_PLAYING || bForcer)
		alSourcePlay(m_pSources[bIndice]);

}


void CMoteurAudio::PlaySoundSequence(unsigned int* pTabSequence, unsigned int nNbSequence)
{
	if(pTabSequence != NULL && nNbSequence > 0 && pTabSequence[0]<Max_SOUND)
	{
		alSourcePlay(m_pSources[pTabSequence[0]]);
		for(int i = 0; i<nNbSequence;)
		{


			ALint Status;
			// R�cup�ration de l'�tat du son
			alGetSourcei(m_pSources[pTabSequence[i]], AL_SOURCE_STATE, &Status);

			if(Status != AL_PLAYING)
			{
				i++;
				if(i<nNbSequence && pTabSequence[i]<Max_SOUND)
					alSourcePlay(m_pSources[pTabSequence[i]]);
			}
				
		}
	}

}


void CMoteurAudio::Destruction()
{
	// R�cup�ration du contexte et du device
	//ALCcontext* Context = alcGetCurrentContext();
	//ALCdevice*  Device  = alcGetContextsDevice(Context);




	// D�sactivation du contexte
	alcMakeContextCurrent(NULL);

	// Destruction du contexte
	alcDestroyContext(m_pAudioContext);

	// Fermeture du device
	alcCloseDevice(m_pAudioDevice);

	SAFE_DELETE(m_pAudioContext);
	SAFE_DELETE(m_pAudioDevice);

}

// Classe pour la gestion de la partie 3D de l'appilcation

#pragma once

#include "VariableGlobale.h"
#include "VariableGlobalDX.h"

#include "Moteur.h"

#include <al.h>
#include <alc.h>
#include <sndfile.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define Max_SOUND 10

class CMoteur;

class CMoteurAudio
{
public:


	CMoteur*			m_pMoteur;			// Pointeur sur le Moteur
	ALCdevice*			m_pAudioDevice;		// Pointeur sur le Device Audio
	ALCcontext*			m_pAudioContext;	// Pointeur sur le Context Audio

	ALuint 		m_pBuffers[Max_SOUND];
	ALuint		m_pSources[Max_SOUND];

	CMoteurAudio();
	CMoteurAudio(CMoteur* pMoteur);
	~CMoteurAudio();

	bool Creation(const char* DeviceName = NULL);
	void Destruction();

	ALCdevice* GetDeviceAudio(){return m_pAudioDevice;}
	ALCcontext* GetContextAudio(){return m_pAudioContext;}
	void GetDevicesDisponible(std::vector<std::string>& Devices);

	bool LoadSound(const std::string& Filename , unsigned int indice);
	void PlaySound(unsigned int indice, bool bForcer = false);
	void PlaySoundSequence(unsigned int* pTabSequence, unsigned int nNbSequence);
	CMoteur* GetMoteur() {return m_pMoteur;}
 
	bool RAZPeripheriqueAudio();
};

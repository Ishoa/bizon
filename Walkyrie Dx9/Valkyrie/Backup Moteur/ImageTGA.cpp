#include "ImageTGA.h"

CImageTGA::CImageTGA()
{
	// On initialise l'entête du fichier à zéro
	ZeroMemory(&m_Entete, sizeof(m_Entete));
	m_Donnee = NULL;
	m_bGris = false;
}

CImageTGA::~CImageTGA()
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}
}

// Constructeur par recopie
CImageTGA::CImageTGA(const CImageTGA& Image)
{
	m_Donnee = NULL;
	m_bGris = Image.m_bGris;
	// Si il y a des données
	if(Image.m_Donnee)
	{
		// On crée une nouvelle image
		if(Creer(Image.m_Entete.ImageWidth, Image.m_Entete.ImageHeight, m_bGris))
		{
			int TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * m_Entete.PixelDepth / 8;
			// Copie des données
			memcpy(m_Donnee, Image.m_Donnee, TailleDonnees);
		}
	}
}


// Operateur d'affectation entre images
CImageTGA& CImageTGA::operator=(const CImageTGA& Image)
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}
	m_bGris = Image.m_bGris;
	// Si il y a des données
	if(Image.m_Donnee)
	{
		// On crée une nouvelle image
		if(Creer(Image.m_Entete.ImageWidth, Image.m_Entete.ImageHeight, m_bGris))
		{
			int TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * m_Entete.PixelDepth / 8;
			// Copie des données
			memcpy(m_Donnee, Image.m_Donnee, TailleDonnees);
		}
	}
	return *this;
}

// Création d'une image vide
bool CImageTGA::Creer(unsigned short Largeur, unsigned short Hauteur, bool bGris)
{
	if(m_Donnee != NULL)
	{
		// on détruit le tableau de données
		delete[] m_Donnee;
	}

	// On initialise l'entête du fichier à zéro
	ZeroMemory(&m_Entete, sizeof(m_Entete));

	// On initialise la l'entête en fonction du type d'image
	m_bGris = bGris;
	if(m_bGris)
	{
		m_Entete.ImageType = 3;
		m_Entete.PixelDepth = 8;
	}
	else
	{
		m_Entete.ImageType = 2;
		m_Entete.PixelDepth = 32;
	}

	// Calcule de la taille des données
	int TailleDonnees = Largeur * Hauteur * m_Entete.PixelDepth / 8;
	m_Entete.ImageWidth = Largeur;
	m_Entete.ImageHeight = Hauteur;
	// Allocation du tableau de données
	m_Donnee = new unsigned char[TailleDonnees];
	return m_Donnee != NULL;
}

bool CImageTGA::Charger(char* NomFichier)
{
	std::ifstream Fichier;

	// On ouvre le ficher en mode binaire
	Fichier.open(NomFichier, std::ios::binary);

	// On vérifie qu'il est bien ouvert
	if(!Fichier.is_open())
	{
		Fichier.close();
		return false;
	}

	// Lecture de l'entête du fichier
	Fichier.read((char*)&m_Entete, sizeof(m_Entete));

	// On vérifie que l'on n'a pas atteins la fin du fichier
	if(Fichier.eof())
	{
		Fichier.close();
		return false;
	}

	// On se positionne sur le début des données de l'image
	Fichier.seekg(sizeof(m_Entete) + m_Entete.ColorMapLength + m_Entete.IDLength, std::ios::beg);

	// Si l'image est en mode RGBA 32bits
	if(m_Entete.ImageType == 2 && m_Entete.PixelDepth == 32)
	{
		int TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * m_Entete.PixelDepth / 8;
		m_Donnee = new unsigned char[TailleDonnees];
		if(m_Donnee != NULL)
		{
			// Chargement des données à partir du fichier
			Charger32bits(&Fichier);
		}
		else
		{
			Fichier.close();
			return false;
		}
	}
	else
	{
		// Si l'image est en mode niveaux de gris 8bits
		if(m_Entete.ImageType == 3 && m_Entete.PixelDepth == 8)
		{
			int TailleDonnees = m_Entete.ImageWidth * m_Entete.ImageHeight * m_Entete.PixelDepth / 8;
			m_Donnee = new unsigned char[TailleDonnees];
			if(m_Donnee != NULL)
			{
				// Chargement des données à partir du fichier
				m_bGris = true;
				Charger8bitsGris(&Fichier);
			}
			else
			{
				Fichier.close();
				return false;
			}
		}
		else
		{
			Fichier.close();
			return false;
		}
	}

	// On à atteins la fin du fichier prématurément
	if(Fichier.eof())
	{
		Fichier.close();
		return false;
	}

	// on ferme le ficher ouvert
	Fichier.close();
	return true;
}

// Chargement d'une image RGBA 32bits à partir du fichier
void CImageTGA::Charger32bits(std::ifstream* Fichier)
{
	int TailleDonneesLire = m_Entete.ImageWidth * m_Entete.ImageHeight * m_Entete.PixelDepth / 8;
	Fichier->read((char*)m_Donnee, TailleDonneesLire);
}

// Chargment d'une image en niveaux de gris 8bits à partir du fichier
void CImageTGA::Charger8bitsGris(std::ifstream* Fichier)
{
	int TailleDonneesLire = m_Entete.ImageWidth * m_Entete.ImageHeight * m_Entete.PixelDepth / 8;
	Fichier->read((char*)m_Donnee, TailleDonneesLire);
}

// Enregistrer l'image dans un fichier
bool CImageTGA::Enregister(char* NomFichier)
{
	EnteteTGA Entete;
	std::ofstream Fichier;
	// On initialise l'entête du fichier à zéro
	ZeroMemory(&Entete, sizeof(Entete));

	// On initialise la l'entête en fonction du type d'image
	if(m_bGris)
	{
		Entete.ImageType = 3;
		Entete.PixelDepth = 8;
	}
	else
	{
		Entete.ImageType = 2;
		Entete.PixelDepth = 32;
	}

	Entete.ImageWidth = m_Entete.ImageWidth;
	Entete.ImageHeight = m_Entete.ImageHeight;

	// On ouvre le ficher en mode binaire
	Fichier.open(NomFichier, std::ios::binary);

	// On vérifie qu'il est bien ouvert
	if(!Fichier.is_open())
	{
		Fichier.close();
		return false;
	}

	// Ecriture de l'entête
	Fichier.write((char*)&Entete, sizeof(Entete));
	// Positionnement sur le début des données de l'image
	Fichier.seekp(sizeof(Entete) + Entete.ColorMapLength + Entete.IDLength, std::ios::beg);
	int TailleDonnees = Entete.ImageWidth * Entete.ImageHeight * Entete.PixelDepth / 8;
	// Ecriture des données de l'image
	Fichier.write((char*)m_Donnee, TailleDonnees);
	// Fermeture du fichier
	Fichier.close();
	return true;
}

// Obtenire la couleur d'un pixel
COLORREF CImageTGA::GetPixel(int X, int Y)
{
	if(X < 0 || Y < 0 || X >= m_Entete.ImageWidth || Y >= m_Entete.ImageHeight)
		return 0;
	if(m_bGris)
		return m_Donnee[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X];
	else
		return ((COLORREF*)m_Donnee)[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X];
}

// Modifier la couleur d'un pixel
void CImageTGA::SetPixel(int X, int Y, COLORREF Couleur)
{
	if(X < 0 || Y < 0 || X >= m_Entete.ImageWidth || Y >= m_Entete.ImageHeight)
		return;
	if(m_bGris)
		m_Donnee[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X] = (unsigned char)(Couleur & 0xFF);
	else
		((COLORREF*)m_Donnee)[m_Entete.ImageWidth * (m_Entete.ImageHeight - 1 - Y) + X] = Couleur;
}
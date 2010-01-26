#include "EnvCollision.h"


CEnvCollision::CEnvCollision()
{
	m_nNbCollision = 0;

	for(int i = 0;i < MAXOBJET;i++)
	{
		m_pTabCollision[i].m_pObjetDeCollision = NULL;
		m_pTabCollision[i].m_nIdObjet = -1;
	}
}

CEnvCollision::~CEnvCollision()
{
	

}

void CEnvCollision::Release()
{

	for(int i = 0;i < MAXOBJET;i++)
	{
			m_pTabCollision[i].m_pObjetDeCollision = NULL;
			m_pTabCollision[i].m_nIdObjet = -1;
	}

	m_nNbCollision = 0;
}

bool CEnvCollision::AddObjetDeCollision(CObjetDeCollision* ObjetDeCollision,CTypeDeCheckCollision eTypeCollision,float fAvatarHeight,CCalculsCollisions::eCollisionType eCollisionType)
{
	if(m_nNbCollision > MAXOBJET)
		return false;
	m_pTabCollision[m_nNbCollision].m_eTypeCheckDeCollision = eTypeCollision;
	m_pTabCollision[m_nNbCollision].m_nIdObjet = m_nNbCollision;
	m_pTabCollision[m_nNbCollision].m_pObjetDeCollision = ObjetDeCollision;
	m_pTabCollision[m_nNbCollision].m_eCollisionType = eCollisionType;
	m_pTabCollision[m_nNbCollision].m_fAvatarHeight = fAvatarHeight;
	m_nNbCollision++;

	return true;
}


D3DXVECTOR3 CEnvCollision::CheckNextPosition( D3DXVECTOR3 CurrentPosition,D3DXVECTOR3 NextPosition)
{

	unsigned int indiceCollision = -1;
	// Sinon, il faut tester s'il y a eu une collision.
	bool      MultipleCollisions = false;
	D3DXPLANE SlidingPlane;
	D3DXPLANE CollisionPlane;


	for ( int i = 0; i < m_nNbCollision; i++ )
	{
		if(m_pTabCollision[i].m_eCollisionType != CCalculsCollisions::NO_COLLISION)
		{
			switch(m_pTabCollision[i].m_eTypeCheckDeCollision)
			{
			case LIST_POLYGON : 
				{
					// On obtient le pointeur sur la liste de polygones de l'objet.
					CPolygonContainer* CurrentPolygon =
						m_pTabCollision[i].m_pObjetDeCollision->GetPolygonList()->GetFirst();

					// On recherche une collision dans toute la liste de polygones.
					while ( CurrentPolygon != NULL )
					{
						// On obtient le polygone courant, et on lui applique une
						// transformation pour le placer dans le bon système d'axes.
						// Une meilleure méthode serait de convertir uniquement la
						// position à tester dans le système d'axes du polygone, en
						// appliquant l'inverse de la matrice du polygone sur la
						// position, afin d'économiser quelques calculs. Mais parce-
						// qu'on devra utiliser le plan du polygone pour le Sliding,
						// plus tard, il vaut mieux pour notre cas convertir les
						// coordonnées du polygone, et obtenir directement le bon plan.
						CPolygon TestPolygon =
							CurrentPolygon->GetPolygon()->ApplyMatrix
							( m_pTabCollision[i].m_pObjetDeCollision->GetWorldMatrix() );

						// On teste ensuite si l'avatar est en collision avec le
						// polygone. Au passage, on récupère le plan de collision,
						// si celui-ci existe. Remarquez que la position à tester est
						// inversée, afin de travailler avec le bon système d'axes.
						if ( CCalculsCollisions::
							Check( NextPosition,
							m_pTabCollision[i].m_fAvatarHeight,
							TestPolygon,
							&SlidingPlane ) == true )
						{
							Console<<"Collision"<<i<<endl;
							// Si une collision a déjà eu lieu, et si le plan trouvé
							// n'est pas identique au plan de la collision précédente,
							// ni à l'inverse du plan de la collision précédente,
							// alors on indique que le joueur est en collision avec
							// plusieurs murs.
							if ( ( SlidingPlane.a !=  CollisionPlane.a   ||
								SlidingPlane.b !=  CollisionPlane.b   ||
								SlidingPlane.c !=  CollisionPlane.c   ||
								SlidingPlane.d !=  CollisionPlane.d ) &&
								( SlidingPlane.a != -CollisionPlane.a   ||
								SlidingPlane.b != -CollisionPlane.b   ||
								SlidingPlane.c != -CollisionPlane.c   ||
								SlidingPlane.d != -CollisionPlane.d ) &&
								indiceCollision        != -1 )
							{
								MultipleCollisions = true;
								Console<<"COllision Multiple"<<endl;
							}

							// On copie le plan de collision.
							CollisionPlane = SlidingPlane;
							
							// Puis, on indique qu'une collision a eu lieu.
							indiceCollision = i;
						}

						// On passe au polygone suivant, pour parcourir toute la liste.
						CurrentPolygon = CurrentPolygon->GetNext();
					}
				}
				break;

			case BOUNDING_BOX :
				{

					// On obtient le pointeur sur la liste de polygones de l'objet.
					CPolygonContainer* CurrentPolygon =
						m_pTabCollision[i].m_pObjetDeCollision->GetPolygonList()->GetFirstBox();

					// On recherche une collision dans toute la liste de polygones.
					while ( CurrentPolygon != NULL )
					{
						// On obtient le polygone courant, et on lui applique une
						// transformation pour le placer dans le bon système d'axes.
						// Une meilleure méthode serait de convertir uniquement la
						// position à tester dans le système d'axes du polygone, en
						// appliquant l'inverse de la matrice du polygone sur la
						// position, afin d'économiser quelques calculs. Mais parce-
						// qu'on devra utiliser le plan du polygone pour le Sliding,
						// plus tard, il vaut mieux pour notre cas convertir les
						// coordonnées du polygone, et obtenir directement le bon plan.
						
										
						D3DXMATRIXA16 matrix =  m_pTabCollision[i].m_pObjetDeCollision->GetWorldMatrix();

						D3DXMATRIXA16 matrixInverse;
						float det=D3DXMatrixDeterminant(&matrix);
						D3DXMatrixInverse(&matrixInverse,&det,&matrix);

						CPolygon* pTestPolygon	= CurrentPolygon->GetPolygon();//->ApplyMatrix( m_pTabCollision[i].m_pObjetDeCollision->GetWorldMatrix() );
						CPolygon TestPolygon	= *pTestPolygon;

						D3DXVECTOR3 NextPositionInverse;
						D3DXVec3TransformCoord(&NextPositionInverse,&NextPosition,&matrixInverse);
					
						// On teste ensuite si l'avatar est en collision avec le
						// polygone. Au passage, on récupère le plan de collision,
						// si celui-ci existe. Remarquez que la position à tester est
						// inversée, afin de travailler avec le bon système d'axes.
						if ( CCalculsCollisions::
							Check( NextPositionInverse,
							m_pTabCollision[i].m_fAvatarHeight,
							TestPolygon,
							&SlidingPlane ) == true )
						{
						
							D3DXVECTOR3 plantrans;
							D3DXVec3TransformCoord(&plantrans,&D3DXVECTOR3(SlidingPlane.a,SlidingPlane.b,SlidingPlane.c),&matrix);
							SlidingPlane.a = plantrans.x;
							SlidingPlane.b = plantrans.y;
							SlidingPlane.c = plantrans.z;
							Console<<SlidingPlane.a<<":"<<SlidingPlane.b<<":"<<SlidingPlane.c<<endl;
							D3DXPlaneNormalize(&SlidingPlane,&SlidingPlane);
							// Si une collision a déjà eu lieu, et si le plan trouvé
							// n'est pas identique au plan de la collision précédente,
							// ni à l'inverse du plan de la collision précédente,
							// alors on indique que le joueur est en collision avec
							// plusieurs murs.
							if ( ( SlidingPlane.a !=  CollisionPlane.a   ||
								SlidingPlane.b !=  CollisionPlane.b   ||
								SlidingPlane.c !=  CollisionPlane.c   ||
								SlidingPlane.d !=  CollisionPlane.d ) &&
								( SlidingPlane.a != -CollisionPlane.a   ||
								SlidingPlane.b != -CollisionPlane.b   ||
								SlidingPlane.c != -CollisionPlane.c   ||
								SlidingPlane.d != -CollisionPlane.d ) &&
								indiceCollision        != -1 )
							{
								MultipleCollisions = true;
								Console<<"COllision Multiple"<<endl;
							}

							

							// On copie le plan de collision.
							
							CollisionPlane = SlidingPlane;

							// Puis, on indique qu'une collision a eu lieu.
							indiceCollision = i;
						}

						// On passe au polygone suivant, pour parcourir toute la liste.
						CurrentPolygon = CurrentPolygon->GetNext();
					}

				}
				break;

			case BOUNDING_SPHERE : break;

			default:break;
			}

		}
	}

	if(indiceCollision == -1)
	{
		return NextPosition;
	}
	else
	{
		if(m_pTabCollision[indiceCollision].m_eCollisionType == CCalculsCollisions::COLLISION_AND_SLIDING )
		{
			if( MultipleCollisions == false )
			{
				// Si une seule collision a été détectée, on corrige la
				// position du joueur en fonction du plan du mur touché.
				// Remarquez que la position est inversée, pour qu'elle
				// corresponde au système d'axes du plan, et que la réponse est
				// à nouveau inversée, pour la reconvertir aux systèmes d'axes
				// de la caméra.

				return ( CCalculsCollisions::GetSlidingPoint( CollisionPlane,
																NextPosition,
																m_pTabCollision[indiceCollision].m_fAvatarHeight ) );
			}

		}
		else 
			return CurrentPosition;

	}

	return CurrentPosition;
}
















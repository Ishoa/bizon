#include "ObjetDeCollision.h"

CObjetDeCollision::CObjetDeCollision()
{
	m_pPolygonList = NULL;
	m_pPolygonListLowDetail = NULL;
}

CObjetDeCollision::~CObjetDeCollision()
{
	// On lib�re les ressources utilis�es par la liste de polygones.
	if ( m_pPolygonList != NULL )
	{
		delete m_pPolygonList;
		m_pPolygonList = NULL;
	}

	if ( m_pPolygonListLowDetail != NULL )
	{
		delete m_pPolygonListLowDetail;
		m_pPolygonListLowDetail = NULL;
	}
}

void CObjetDeCollision::Release()
{
	if(m_pPolygonList != NULL)
	{
		m_pPolygonList->ReleaseList();
	}

	if(m_pPolygonListLowDetail != NULL)
	{
		m_pPolygonListLowDetail->ReleaseList();
	}
}


// Obtient les donn�es des polygones de l'objet.
CPolygonList* CObjetDeCollision::GetPolygonList()
{
	return m_pPolygonList;
}

// Obtient les donn�es des polygones de l'objet.
CPolygonList* CObjetDeCollision::GetPolygonListLowDetail()
{
	return m_pPolygonListLowDetail;
}


// Obtient la bo�te de collisions de l'objet.
CBoundingBox CObjetDeCollision::GetBoundingBox()
{
	if ( m_pPolygonList == NULL )
	{
		return CBoundingBox();
	}

	return m_pPolygonList->GetBoundingBox();
}


// Obtient la sphere de collisions de l'objet.
CBoundingSphere CObjetDeCollision::GetBoundingSphere()
{
	if ( m_pPolygonList == NULL )
	{
		return CBoundingSphere();
	}

	return m_pPolygonList->GetBoundingSphere();
}



// Cette fonction teste la validit� de la prochaine position.
D3DXVECTOR3 CObjetDeCollision::CheckNextPosition( D3DXVECTOR3 CurrentPosition, D3DXVECTOR3 NextPosition,float fAvatarHeight, CCalculsCollisions::eCollisionType p_CollisionType)
{
	// Teste si le moteur de collision est activ�.
	if ( p_CollisionType == CCalculsCollisions::NO_COLLISION )
	{
		// Si le moteur de collision est ignor�, on retourne simplement la
		// prochaine position.
		return NextPosition;
	}
	else
	{
		// Sinon, il faut tester s'il y a eu une collision.
		bool      Collision          = false;
		bool      MultipleCollisions = false;
		D3DXPLANE SlidingPlane;
		D3DXPLANE CollisionPlane;

		// On teste tous les murs du terrain de jeu. Dans un programme lourd,
		// il faudrait r�duire au maximum le nombre de polygones � tester, en
		// utilisant par exemple un arbre BSP (Binary Space Partitionning).
		// Ici, il s'agit d'une d�mo, donc le code doit rester simple, et le
		// faible nombre de polygones � tester (22 au total) permet d'ignorer
		// ce test. Remarquez tout de m�me que le sol et le plafond ne sont pas
		// test�s, puisqu'il est impossible d'entrer en collision avec eux.

		// On obtient le pointeur sur la liste de polygones de l'objet.
		CPolygonContainer* CurrentPolygon =
			m_pPolygonList->GetFirst();

		// On recherche une collision dans toute la liste de polygones.
		while ( CurrentPolygon != NULL )
		{
			// On obtient le polygone courant, et on lui applique une
			// transformation pour le placer dans le bon syst�me d'axes.
			// Une meilleure m�thode serait de convertir uniquement la
			// position � tester dans le syst�me d'axes du polygone, en
			// appliquant l'inverse de la matrice du polygone sur la
			// position, afin d'�conomiser quelques calculs. Mais parce-
			// qu'on devra utiliser le plan du polygone pour le Sliding,
			// plus tard, il vaut mieux pour notre cas convertir les
			// coordonn�es du polygone, et obtenir directement le bon plan.
			CPolygon TestPolygon =
				CurrentPolygon->GetPolygon()->ApplyMatrix(GetWorldMatrix());

			// On teste ensuite si l'avatar est en collision avec le
			// polygone. Au passage, on r�cup�re le plan de collision,
			// si celui-ci existe. Remarquez que la position � tester est
			// invers�e, afin de travailler avec le bon syst�me d'axes.
			if ( CCalculsCollisions::
				Check( NextPosition,
				fAvatarHeight,
				TestPolygon,
				&SlidingPlane ) == true )
			{
				// Si une collision a d�j� eu lieu, et si le plan trouv�
				// n'est pas identique au plan de la collision pr�c�dente,
				// ni � l'inverse du plan de la collision pr�c�dente,
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
					Collision        == true )
				{
					MultipleCollisions = true;
				}

				// On copie le plan de collision.
				CollisionPlane = SlidingPlane;
				Console<<"COllision"<<endl;
				// Puis, on indique qu'une collision a eu lieu.
				Collision = true;
			}

			// On passe au polygone suivant, pour parcourir toute la liste.
			CurrentPolygon = CurrentPolygon->GetNext();
		}

		// On teste l'�tat du moteur de collision.
		if ( p_CollisionType == CCalculsCollisions::COLLISION_ONLY )
		{
			// Si le moteur doit tester uniquement les collisions, on teste
			// les valeurs retourn�es par la fonction de collision.
			if ( Collision == false )
			{
				// Si aucune collision n'a �t� d�tect�e, alors la prochaine
				// position est valide. Dans ce cas, on la retoune simplement.
				return NextPosition;
			}

			// Si une collision a �t� trouv�e, la nouvelle position n'est
			// pas valide. Dans ce cas, on reste � la position courante.
			return CurrentPosition;
		}
		else
		{
			// Si le moteur doit tester les collisions et g�rer le sliding,
			// on teste les valeurs retourn�es par la fonction de collision.
			if ( Collision == false )
			{
				// Si aucune collision n'a �t� d�tect�e, alors la prochaine
				// position est valide. Dans ce cas, on la retourne simplement.
				return NextPosition;
			}
			else if ( Collision == true && MultipleCollisions == false )
			{
				// Si une seule collision a �t� d�tect�e, on corrige la
				// position du joueur en fonction du plan du mur touch�.
				// Remarquez que la position est invers�e, pour qu'elle
				// corresponde au syst�me d'axes du plan, et que la r�ponse est
				// � nouveau invers�e, pour la reconvertir aux syst�mes d'axes
				// de la cam�ra.
				return ( CCalculsCollisions::GetSlidingPoint( CollisionPlane,
					NextPosition,
					fAvatarHeight ) );
			}

			// Si plus d'une collision a �t� trouv�e, la nouvelle position
			// n'est pas valide. Dans ce cas, on reste � la position courante.
			return CurrentPosition;
		}
	}

	// Ce point du code n'est jamais atteint, mais il vaut mieux le traiter
	// quand m�me, pour �viter des erreurs de compilation.
	return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}
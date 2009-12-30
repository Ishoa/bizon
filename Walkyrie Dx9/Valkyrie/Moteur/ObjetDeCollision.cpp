#include "ObjetDeCollision.h"

CObjetDeCollision::CObjetDeCollision()
{
	m_pPolygonList = NULL;
	m_pPolygonListLowDetail = NULL;
}

CObjetDeCollision::~CObjetDeCollision()
{
	// On libère les ressources utilisées par la liste de polygones.
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


// Obtient les données des polygones de l'objet.
CPolygonList* CObjetDeCollision::GetPolygonList()
{
	return m_pPolygonList;
}

// Obtient les données des polygones de l'objet.
CPolygonList* CObjetDeCollision::GetPolygonListLowDetail()
{
	return m_pPolygonListLowDetail;
}


// Obtient la boîte de collisions de l'objet.
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



// Cette fonction teste la validité de la prochaine position.
D3DXVECTOR3 CObjetDeCollision::CheckNextPosition( D3DXVECTOR3 CurrentPosition, D3DXVECTOR3 NextPosition,float fAvatarHeight, CCalculsCollisions::eCollisionType p_CollisionType)
{
	// Teste si le moteur de collision est activé.
	if ( p_CollisionType == CCalculsCollisions::NO_COLLISION )
	{
		// Si le moteur de collision est ignoré, on retourne simplement la
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
		// il faudrait réduire au maximum le nombre de polygones à tester, en
		// utilisant par exemple un arbre BSP (Binary Space Partitionning).
		// Ici, il s'agit d'une démo, donc le code doit rester simple, et le
		// faible nombre de polygones à tester (22 au total) permet d'ignorer
		// ce test. Remarquez tout de même que le sol et le plafond ne sont pas
		// testés, puisqu'il est impossible d'entrer en collision avec eux.

		// On obtient le pointeur sur la liste de polygones de l'objet.
		CPolygonContainer* CurrentPolygon =
			m_pPolygonList->GetFirst();

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
				CurrentPolygon->GetPolygon()->ApplyMatrix(GetWorldMatrix());

			// On teste ensuite si l'avatar est en collision avec le
			// polygone. Au passage, on récupère le plan de collision,
			// si celui-ci existe. Remarquez que la position à tester est
			// inversée, afin de travailler avec le bon système d'axes.
			if ( CCalculsCollisions::
				Check( NextPosition,
				fAvatarHeight,
				TestPolygon,
				&SlidingPlane ) == true )
			{
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

		// On teste l'état du moteur de collision.
		if ( p_CollisionType == CCalculsCollisions::COLLISION_ONLY )
		{
			// Si le moteur doit tester uniquement les collisions, on teste
			// les valeurs retournées par la fonction de collision.
			if ( Collision == false )
			{
				// Si aucune collision n'a été détectée, alors la prochaine
				// position est valide. Dans ce cas, on la retoune simplement.
				return NextPosition;
			}

			// Si une collision a été trouvée, la nouvelle position n'est
			// pas valide. Dans ce cas, on reste à la position courante.
			return CurrentPosition;
		}
		else
		{
			// Si le moteur doit tester les collisions et gérer le sliding,
			// on teste les valeurs retournées par la fonction de collision.
			if ( Collision == false )
			{
				// Si aucune collision n'a été détectée, alors la prochaine
				// position est valide. Dans ce cas, on la retourne simplement.
				return NextPosition;
			}
			else if ( Collision == true && MultipleCollisions == false )
			{
				// Si une seule collision a été détectée, on corrige la
				// position du joueur en fonction du plan du mur touché.
				// Remarquez que la position est inversée, pour qu'elle
				// corresponde au système d'axes du plan, et que la réponse est
				// à nouveau inversée, pour la reconvertir aux systèmes d'axes
				// de la caméra.
				return ( CCalculsCollisions::GetSlidingPoint( CollisionPlane,
					NextPosition,
					fAvatarHeight ) );
			}

			// Si plus d'une collision a été trouvée, la nouvelle position
			// n'est pas valide. Dans ce cas, on reste à la position courante.
			return CurrentPosition;
		}
	}

	// Ce point du code n'est jamais atteint, mais il vaut mieux le traiter
	// quand même, pour éviter des erreurs de compilation.
	return D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
}
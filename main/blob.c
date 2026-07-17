#include "blob.h"
#include <math.h>
#include <stdlib.h>

/**
 * @brief Nombre de formules de trajectoires disponibles.
 *
 * Définit le nombre total de modes de déplacement utilisables
 * par le moteur de blobs.
 */
#define NUM_MODS 6

//--------------------------------------------------
// Prototypes privés
//--------------------------------------------------

static Point getBlobCircle(float t);
static Point getBlobFlower(float t);
static Point getBlobFigure8(float t);
static Point getBlobOval(float t);
static Point getBlobStar(float t);
static Point getBlobRotatingOval(float t);

//--------------------------------------------------
// Variables privées
//--------------------------------------------------

static uint8_t mode;
static int16_t x_min;
static int16_t x_max;
static int16_t y_min;
static int16_t y_max;

//--------------------------------------------------
// Fonctions publiques
//--------------------------------------------------

void blob_init(
    int16_t xmin,
    int16_t xmax,
    int16_t ymin,
    int16_t ymax
)
{
    x_min=xmin;
    x_max=xmax;
    y_min=ymin;
    y_max=ymax;
    srand(esp_random());
}

Point getBlobPosition(float t)
{
	
    switch(mode)
    {
		case 0:
            return getBlobCircle(t);

        case 1:
            return getBlobFlower(t);

        case 2:
            return getBlobStar(t);

		case 3:
			return getBlobFigure8(t);
		
		case 4:
			return getBlobOval(t);
			
		case 5:
			return getBlobRotatingOval(t);
			
        default:
            return getBlobCircle(t);
    }
}

float switchBlob(int8_t blobFormula){
	float max_display_time[]={60,30,60,60,60};
	
	if(blobFormula>=0 && blobFormula<NUM_MODS){
		mode=blobFormula;
	}else {
		int8_t oldMode = mode;
		mode = rand() % NUM_MODS;
		if (mode == oldMode) {
			mode = (mode + 1) % NUM_MODS;
		}
	}
	
	return max_display_time[mode];
}

//--------------------------------------------------
// Formules privées
//--------------------------------------------------

/**
 * @brief Calcule une trajectoire en forme d'étoile.
 *
 * Génère une position périodique dont le rayon varie en fonction
 * de plusieurs oscillations sinusoïdales afin de créer un mouvement
 * rappelant une étoile animée.
 *
 * Cette fonction est interne au module blob et n'est pas accessible
 * depuis les autres fichiers.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position calculée du blob en pixels.
 */
static Point getBlobStar(float t)
{
    Point p;

    float speed = 1.5f*GLOBAL_SPEED_FACTORE;
    float angle = t * speed;


    // Zone disponible
    float rx = (x_max - x_min) / 2.0f - 2.0f;
    float ry = (y_max - y_min) / 2.0f - 2.0f;


    // Centre
    float cx = (x_max + x_min) / 2.0f;
    float cy = (y_max + y_min) / 2.0f;


    // Rayon qui respire
    float breath =
        0.75f
        + 0.20f * sinf(t * 0.25f);


    float rMax = rx * breath;
    float rMin = rMax * 0.45f;


    // Etoile 5 branches
    float star =
        (sinf(5.0f * angle) + 1.0f) * 0.5f;


    float radius =
        rMin + (rMax - rMin) * star;


    // Rotation + mouvement
    p.x = (int16_t)(cx + radius * cosf(angle));

    p.y = (int16_t)(cy + (radius * ry / rx) * sinf(angle));


    return p;
}

/**
 * @brief Génère une trajectoire en forme de fleur.
 *
 * Calcule une position périodique basée sur une variation du rayon
 * en fonction de plusieurs oscillations sinusoïdales. Le rayon varie
 * au cours du temps afin de créer un mouvement rappelant une fleur
 * qui s'ouvre et se referme.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position calculée du blob en pixels.
 */
static Point getBlobFlower(float t)
{
    Point p;

    float speed = 1.5f*GLOBAL_SPEED_FACTORE;
    float angle = t * speed;


    // Dimensions disponibles pour le sprite
    float rx = (x_max - x_min) / 2.0f - 2.0f;
    float ry = (y_max - y_min) / 2.0f - 2.0f;


    // Centre de l'écran utile
    float cx = (x_max + x_min) / 2.0f;
    float cy = (y_max + y_min) / 2.0f;


    // Respiration lente du rayon
    float radius =
        rx * 0.75f
        + rx * 0.12f * sinf(t * 0.18f)
        + rx * 0.05f * sinf(t * 0.43f);


    // Déformation type blob/fleur
    float petal =
        1.0f
        + 0.10f * sinf(angle * 3.0f + t * 0.12f);


    radius *= petal;


    // Limite de sécurité écran
    if (radius > rx)
        radius = rx;

    if (radius < 0)
        radius = 0;


    // Trajectoire
    p.x = (int16_t)(cx + radius * cosf(angle));

    p.y = (int16_t)(cy + (radius * ry / rx) * sinf(angle * 1.15f));


    return p;
}

/**
 * @brief Génère une trajectoire circulaire.
 *
 * Calcule une position périodique autour du centre de l'écran
 * avec un rayon constant ou variable selon l'implémentation.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position X/Y du sprite en pixels.
 */
static Point getBlobCircle(float t)
{
    Point p;

    float speed = 2.0f*GLOBAL_SPEED_FACTORE;   // vitesse de rotation rad/s

    float angle = t * speed;


    // limites écran
    float r = (x_max - x_min) / 2.0f - 1.0f;
    
    float cx = (x_max + x_min) / 2.0f;
    float cy = (y_max + y_min) / 2.0f;


    // rayon qui respire
    float radius = 0.5f + 0.5f * sinf(t * 0.25f);

    // conversion 0..1 vers taille réelle
    radius = 30.0f + radius * (r - 30.0f);


    p.x = cx + radius * cosf(angle);
    p.y = cy + radius * sinf(angle);

    return p;
}

/**
 * @brief Génère une trajectoire en forme de huit.
 *
 * Calcule une trajectoire périodique basée sur deux oscillations
 * sinusoïdales de fréquences différentes afin de produire un mouvement
 * en forme de huit. Un déphasage peut être appliqué entre les axes X
 * et Y pour modifier la forme et la dynamique du déplacement.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position X/Y du sprite en pixels.
 */
static Point getBlobFigure8(float t)
{
    Point p;

    float speed = 2.0f * M_PI / 8 *GLOBAL_SPEED_FACTORE ; // Un quart tour par secondes

    float a = t * speed;
	
	
	float rx = (x_max - x_min) / 2.0f - 1.0f;
    float ry = (y_max - y_min) / 2.0f - 1.0f;

    float cx = (x_max + x_min) / 2.0f;
    float cy = (y_max + y_min) / 2.0f;

    // déformation lente
    float deform = 0.90f + 0.10f * sinf(t * 0.06f);

    // torsion lente
    float phase = 1.5f * sinf(t * 0.04f);

    float x = cx + rx * deform * sinf(a);
    float y = cy + ry * sinf(2.0f * a + phase);

    p.x = (int16_t)x;
    p.y = (int16_t)y;

    return p;
}

/**
 * @brief Génère une trajectoire elliptique avec variation des rayons.
 *
 * Calcule une position périodique autour d'un centre en utilisant
 * deux rayons indépendants sur les axes X et Y. Les dimensions de
 * l'ellipse peuvent évoluer dans le temps afin de créer un effet
 * de respiration ou de déformation du mouvement.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position X/Y du sprite en pixels.
 */
static Point getBlobOval(float t)
{
    Point p;

    float speed = 2.0f*GLOBAL_SPEED_FACTORE;
    float angle = t * speed;

    // Zone disponible
    float rx = (x_max - x_min) / 2.0f - 2.0f;
    float ry = (y_max - y_min) / 2.0f - 2.0f;

    // Centre
    float cx = (x_max + x_min) / 2.0f;
    float cy = (y_max + y_min) / 2.0f;

    // Rayons qui respirent indépendamment
    float radiusX =
        rx * (0.75f + 0.20f * sinf(t * 0.18f));

    float radiusY =
        ry * (0.75f + 0.20f * cosf(t * 0.23f));

    p.x = (int16_t)(cx + radiusX * cosf(angle));
    p.y = (int16_t)(cy + radiusY * sinf(angle));

    return p;
}

/**
 * @brief Génère une trajectoire elliptique rotative.
 *
 * Calcule une trajectoire en forme d'ellipse dont l'orientation
 * varie progressivement dans le temps. La rotation de l'ellipse
 * permet de modifier les axes de déplacement afin que le blob
 * atteigne alternativement les bords horizontaux et verticaux
 * de la zone d'affichage.
 *
 * Le centre de la trajectoire reste fixe afin de garantir que le
 * sprite reste toujours dans les limites de l'écran.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position X/Y du sprite en pixels.
 */
static Point getBlobRotatingOval(float t)
{
    Point p;

    // Vitesses de l'animation
    float speed = 1.5f*GLOBAL_SPEED_FACTORE;       // vitesse de parcours de l'ellipse
    float rotateSpeed = 0.35f; // vitesse de rotation de l'ellipse


    float cx = (x_max + x_min) / 2.0f;
    float cy = (y_max + y_min) / 2.0f;


    // Taille maximale disponible
    float rx = (x_max - x_min) / 2.0f;
    float ry = (y_max - y_min) / 2.0f;


    // Rotation de l'ellipse
    float rot = t * rotateSpeed;


    // Mouvement sur l'ellipse
    float a = t * speed;


    // Coordonnées locales
    float x = rx * cosf(a);
    float y = ry * sinf(a);


    // Rotation du repère
    p.x = (int16_t)(cx + x*cosf(rot) - y*sinf(rot));
    p.y = (int16_t)(cy + x*sinf(rot) + y*cosf(rot));


    return p;
}
/**
 * @brief Génère une trajectoire elliptique rotative.
 *
 * Calcule une trajectoire en forme d'ellipse dont l'orientation
 * varie progressivement dans le temps. La rotation de l'ellipse
 * permet de modifier les axes de déplacement afin que le blob
 * atteigne alternativement les bords horizontaux et verticaux
 * de la zone d'affichage.
 *
 * Le centre de la trajectoire reste fixe afin de garantir que le
 * sprite reste toujours dans les limites de l'écran.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position X/Y du sprite en pixels.
 */
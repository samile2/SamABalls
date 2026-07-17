#pragma once

#include <stdint.h>
#include "esp_random.h"

/**
 * @brief Sélection spéciale : choisir une formule aléatoire.
 *
 * Valeur a passer à la function switchBlob pour choisir 
 * une formule aléatoirement
 *
 */
#define RANDOM_BLOB_FORMULA -1
#define GLOBAL_SPEED_FACTORE 1

/**
 * @brief Coordonnées d'un point.
 */
typedef struct
{
    int16_t x;
    int16_t y;

} Point;

/**
 * @brief Initialise le moteur de trajectoires des blobs.
 *
 * Définit les limites de déplacement du sprite et initialise
 * les paramètres internes utilisés par les différentes formules
 * de trajectoire. Les coordonnées correspondent à la zone dans
 * laquelle le coin supérieur gauche du sprite est autorisé à se
 * déplacer.
 *
 * @param xmin Coordonnée X minimale.
 * @param xmax Coordonnée X maximale.
 * @param ymin Coordonnée Y minimale.
 * @param ymax Coordonnée Y maximale.
 */
void blob_init(
    int16_t xmin,
    int16_t xmax,
    int16_t ymin,
    int16_t ymax
);

/**
 * @brief Calcule la position actuelle du blob selon la trajectoire active.
 *
 * Évalue la formule de déplacement actuellement sélectionnée et retourne
 * la position du sprite pour un instant donné. Le paramètre temps permet
 * de générer une animation continue et indépendante de la fréquence de
 * rafraîchissement de l'affichage.
 *
 * @param t Temps écoulé en secondes.
 *
 * @return Position du blob contenant les coordonnées X et Y en pixels.
 */
Point getBlobPosition(float t);

/**
 * @brief Change la formule de trajectoire du blob.
 *
 * Sélectionne une nouvelle formule de déplacement. Si
 * RANDOM_BLOB_FORMULA est passé, une formule est choisie
 * aléatoirement.
 *
 * @param blobFormula Numéro de la formule ou RANDOM_BLOB_FORMULA.
 * @return Temps maximum d'affichage de cette formule (secondes).
 */
float switchBlob(int8_t blobFormula);
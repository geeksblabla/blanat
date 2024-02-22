import csv
from collections import defaultdict

def traiter_donnees(chemin_fichier):
    prix_par_ville = defaultdict(float)
    prix_par_produit_et_ville = defaultdict(lambda: defaultdict(list))

    with open(chemin_fichier, newline='', encoding='utf-8') as csvfile:
        lecteur = csv.reader(csvfile, delimiter=',')
        for ligne in lecteur:
            ville, produit, prix = ligne
            prix = float(prix)

            prix_par_ville[ville] += prix
            prix_par_produit_et_ville[produit][ville].append(prix)

    return prix_par_ville, prix_par_produit_et_ville


def trouver_moins_cher(prix_par_ville, prix_par_produit_et_ville):
    villes_triees = sorted(prix_par_ville.items(), key=lambda x: x[1])
    ville_moins_chere, prix_total = villes_triees[0]

    prix_produit_ville = {produit: min(prix[ville_moins_chere]) for produit, prix in prix_par_produit_et_ville.items()}

    produits_moins_chers = sorted(prix_produit_ville.items(), key=lambda x: (x[1], x[0]))[:5]

    return ville_moins_chere, prix_total, produits_moins_chers


def ecrire_resultat(chemin_fichier, ville_moins_chere, prix_total, produits_moins_chers):
    with open(chemin_fichier, 'w', encoding='utf-8') as fichier_sortie:
        fichier_sortie.write(f"{ville_moins_chere} {prix_total:.2f}\n")
        for produit, prix in produits_moins_chers:
            fichier_sortie.write(f"{produit} {prix:.2f}\n")


if __name__ == "__main__":
    chemin_fichier_entree = "input.txt"
    chemin_fichier_sortie = "./output.txt"

    prix_par_ville, prix_par_produit_et_ville = traiter_donnees(chemin_fichier_entree)
    ville_moins_chere, prix_total, produits_moins_chers = trouver_moins_cher(prix_par_ville, prix_par_produit_et_ville)
    ecrire_resultat(chemin_fichier_sortie, ville_moins_chere, prix_total, produits_moins_chers)

reset
set terminal png
set output "Graphe.png"
set title textcolor rgb "blue" "Temps d'execution en fonction du nombre de thread"
set grid # Grille de fond
set label "Nombre de threads"
set ylabel "Temps d'execution"
set style fill transparent solid 0.3# Colorisation
plot "a.dat" using 1:2 with lines

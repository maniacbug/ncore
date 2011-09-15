BEGIN {
RS="";
OFS=",";
}
$2 ~ /\.\./ {
  gsub(/executed:/,"",$4)
  gsub(/'/,"",$2)
  printf ("%-20s %7s %3s\n", $2,$4,$6 )
  total += $6
}
END {
  print "Total lines:" total
}

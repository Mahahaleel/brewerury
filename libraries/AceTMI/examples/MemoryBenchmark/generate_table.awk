#!/usr/bin/gawk -f
#
# Usage: generate_table.awk < ${board}.txt
#
# Takes the file generated by collect.sh and generates an ASCII
# table that can be inserted into the README.md.

BEGIN {
  labels[0] = "baseline"
  labels[1] = "SimpleTmiInterface";
  labels[2] = "SimpleTmiFastInterface";
  record_index = 0
}
{
  u[record_index]["flash"] = $2
  u[record_index]["ram"] = $4
  record_index++
}
END {
  NUM_ENTRIES = record_index

  # Calculate the flash and memory deltas from baseline
  base_flash = u[0]["flash"]
  base_ram = u[0]["ram"]
  for (i = 0; i < NUM_ENTRIES; i++) {
    if (u[i]["flash"] != "-1") {
      u[i]["d_flash"] = u[i]["flash"] - base_flash
      u[i]["d_ram"] = u[i]["ram"] - base_ram
    } else {
      u[i]["d_flash"] = -1
      u[i]["d_ram"] = -1
    }
  }

  printf("+--------------------------------------------------------------+\n")
  printf("| functionality                   |  flash/  ram |       delta |\n")
  printf("|---------------------------------+--------------+-------------|\n")
  printf("| %-31s | %6d/%5d | %5d/%5d |\n",
    labels[0], u[0]["flash"], u[0]["ram"], u[0]["d_flash"], u[0]["d_ram"])
  for (i = 1 ; i < NUM_ENTRIES; i++) {
    if (u[i]["flash"] == "-1") continue

    if (labels[i] ~ /^SimpleTmiInterface/) {
      printf(\
        "|---------------------------------+--------------+-------------|\n")
    }
    printf("| %-31s | %6d/%5d | %5d/%5d |\n",
        labels[i], u[i]["flash"], u[i]["ram"], u[i]["d_flash"], u[i]["d_ram"])
  }
  printf("+--------------------------------------------------------------+\n")
}

#!/usr/bin/gawk -f
#
# Usage: generate_table.awk < ${board}.txt
#
# Takes the *.txt file generated by AutoBenchmark.ino and generates an ASCII
# table that can be inserted into the README.md. Collects both sizeof()
# information as well as CPU benchmarks.

BEGIN {
  # Set to 1 when 'SIZEOF' is detected
  collect_sizeof = 0

  # Set to 1 when 'BENCHMARKS' is detected
  collect_benchmarks = 0
}

/^SIZEOF/ {
  collect_sizeof = 1
  collect_benchmarks = 0
  sizeof_index = 0
  next
}

/^BENCHMARKS/ {
  collect_sizeof = 0
  collect_benchmarks = 1
  benchmark_index = 0
  next
}

!/^END/ {
  if (collect_sizeof) {
    s[sizeof_index] = $0
    sizeof_index++
  }
  if (collect_benchmarks) {
    u[benchmark_index]["name"] = $1
    u[benchmark_index]["min"] = $2
    u[benchmark_index]["avg"] = $3
    u[benchmark_index]["max"] = $4
    u[benchmark_index]["samples"] = $5
    benchmark_index++
  }
}

END {
  TOTAL_BENCHMARKS = benchmark_index
  TOTAL_SIZEOF = sizeof_index

  printf("Sizes of Objects:\n")
  for (i = 0; i < TOTAL_SIZEOF; i++) {
    print s[i]
  }

  print ""
  print "CPU:"

  printf("+-------------------------------------------+-------------------+---------+\n")
  printf("| Functionality                             |   min/  avg/  max | samples |\n")
  for (i = 0; i < TOTAL_BENCHMARKS; i++) {
    name = u[i]["name"]
    if (name ~ /^Direct\(4\)$/ \
        || name ~ /^Hybrid\(4,HardSpi\)/ \
        || name ~ /^Hc595\(8,HardSpi\)/ \
        || name ~ /^Tm1637\(4,SimpleTmi,100us\)/ \
        || name ~ /^Tm1637\(4,SimpleTmi,5us\)/ \
        || name ~ /^Tm1637\(6,SimpleTmi,100us\)/ \
        || name ~ /^Max7219\(8,HardSpi\)/ \
        || name ~ /^Ht16k33\(4,TwoWire,100kHz)/ \
    ) {
      printf("|-------------------------------------------+-------------------+---------|\n")
    }

    printf("| %-41s | %5d/%5d/%5d |    %4d |\n",
      u[i]["name"], u[i]["min"], u[i]["avg"], u[i]["max"], u[i]["samples"])
  }
  printf("+-------------------------------------------+-------------------+---------+\n")
}

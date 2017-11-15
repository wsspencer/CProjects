#!/bin/bash
FAIL=0

# Function to run the program against a test case and check
# its output and exit status for correct behavior
testUn-utf8() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt stderr.txt

  if [ $TESTNO -eq 15 ]; then
      echo "Test $TESTNO: ./un-utf8 too many arguments > output.txt 2> stderr.txt"
      ./un-utf8 too many arguments > output.txt 2> stderr.txt
      STATUS=$?
  else
      echo "Test $TESTNO: ./un-utf8 input-$TESTNO.txt > output.txt 2> stderr.txt"
      ./un-utf8 input-$TESTNO.txt > output.txt 2> stderr.txt
      STATUS=$?
  fi

  # Make sure the program exited successfully
  if [ $STATUS -ne $ESTATUS ]
  then
      echo "**** Test $TESTNO FAILED - bad exit status (expected $ESTATUS, got $STATUS)"
      FAIL=1
      return 1
  fi
  
  # Make sure any output to standard out looks right.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  if [ -f expected-stderr-$TESTNO.txt ] ; then
      if ! diff -q expected-stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
      then
	  echo "**** Test $TESTNO FAILED - output to stderr didn't match expected"
	  FAIL=1
	  return 1
      fi
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

if [ -x un-utf8 ] ; then
    testUn-utf8 01 0
    testUn-utf8 02 0
    testUn-utf8 03 0
    testUn-utf8 04 0
    testUn-utf8 05 0
    testUn-utf8 06 0
    testUn-utf8 07 0
    testUn-utf8 07 0
    testUn-utf8 09 0
    testUn-utf8 10 0
    testUn-utf8 11 0
    testUn-utf8 12 0
    testUn-utf8 13 0
    testUn-utf8 14 0
    testUn-utf8 15 1
    testUn-utf8 16 1
else
    echo "**** Your program didn't compile successfully, so we couldn't test it."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi

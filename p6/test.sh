#!/bin/bash
FAIL=0

# Function to run the program against a test case and check
# its output and exit status for correct behavior
testNonde() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt stderr.txt

  echo "Test $TESTNO: ./nonde script-$TESTNO.txt > output.txt 2> stderr.txt"
  ./nonde script-$TESTNO.txt > output.txt 2> stderr.txt
  STATUS=$?

  # Make sure the program exited with the right exit status.
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

if [ -x nonde ] ; then
    # Test 3 depends on this being set before the program runs.
    export MESSAGE="Hello World"
    
    testNonde 01 0
    testNonde 02 0
    testNonde 03 0
    testNonde 04 0
    testNonde 05 0
    testNonde 06 0
    testNonde 07 0
    testNonde 08 0
    testNonde 09 0
    testNonde 10 0
    testNonde 11 0
    testNonde 12 0
    testNonde 13 0
    testNonde 14 0
    testNonde 15 0
    testNonde 16 1
    testNonde 17 1
    testNonde 18 1
    testNonde 19 1
    testNonde 20 1
    testNonde 21 1
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

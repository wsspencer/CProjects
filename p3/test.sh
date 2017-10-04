#!/bin/bash
FAIL=0

# Function to run the program against a test case and check
# its output and exit status for correct behavior
testPuzzle() {
  TESTNO=$1
  ESTATUS=$2

  rm -f output.txt stderr.txt

  # Does this test use a config file?
  if [ "$#" -eq 3 ] ; then
      echo "Test $TESTNO: ./puzzle $3 < input-$TESTNO.txt > output.txt 2> stderr.txt"
      ./puzzle $3 < input-$TESTNO.txt > output.txt 2> stderr.txt
      STATUS=$?
  elif [ "$#" -eq 4 ] ; then
      echo "Test $TESTNO: ./puzzle $3 $4 < input-$TESTNO.txt > output.txt 2> stderr.txt"
      ./puzzle $3 $4 < input-$TESTNO.txt > output.txt 2> stderr.txt
      STATUS=$?
  else
      echo "Test $TESTNO: ./puzzle < input-$TESTNO.txt > output.txt 2> stderr.txt"
      ./puzzle < input-$TESTNO.txt > output.txt 2> stderr.txt
      STATUS=$?
  fi

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]
  then
      echo "**** Test $TESTNO FAILED - incorrect exit status. Expected: $ESTATUS Got: $STATUS"
      FAIL=1
      return 1
  fi
  
  # Make sure the error message is right, if it was supposed to print
  # an error message.
  if [ -f stderr-$TESTNO.txt ]; then
    if ! diff -q stderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
    then
      echo "**** Test $TESTNO FAILED - error message is incorrect."
      FAIL=1
      return 1
    fi
  else
    if [ -s stderr.txt ]; then
      echo "**** Test $TESTNO FAILED - generated unexpected output to stderr."
      FAIL=1
      return 1
    fi
  fi

  # Make sure the output matches the expected output.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

if [ -x puzzle ] ; then
    testPuzzle 1 0
    testPuzzle 2 0
    testPuzzle 3 0
    testPuzzle 4 0
    testPuzzle 5 0
    testPuzzle 6 0
    testPuzzle 7 0
    testPuzzle 8 0 config-8.txt
    testPuzzle 9 0 config-9.txt
    testPuzzle 10 0 config-10.txt
    testPuzzle 11 0
    testPuzzle 12 0
    testPuzzle 13 0
    testPuzzle 14 0
    testPuzzle 15 0
    testPuzzle 16 1 config-16.txt config-16.txt
    testPuzzle 17 1 missing-file.txt
    testPuzzle 18 1 config-18.txt
    testPuzzle 19 1 config-19.txt
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

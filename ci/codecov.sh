#!/bin/bash

make -f ./tests/Makefile $1
rc=$?
if [[ $rc = 0 ]]
then
  # Uploading report to CodeCov
  bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports"
fi

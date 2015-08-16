#!/bin/bash

# ==============================================================================
# Description: Runs the latest application of Univursa, if applicable. Fails if
#              the file doesn't exist. FOR LINUX
# Date: August 16, 2015
# ==============================================================================

./Univursa 2> run-stderr.log | tee run-stdout.log
echo ------------ RUN ERROR/WARNING NOTIFICATIONS -------------
cat run-stderr.log
echo ----------------------------------------------------------

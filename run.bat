::==============================================================================
:: Description: Runs the latest application of Univursa, if applicable. Fails if
::              the file doesn't exist.
:: Date: November 16, 2013
::==============================================================================
@echo off

Univursa.exe 2> run-stderr.log | tee run-stdout.log
echo ------------ RUN ERROR/WARNING NOTIFICATIONS -------------
cat run-stderr.log
echo ----------------------------------------------------------

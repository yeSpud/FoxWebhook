# Runs the script headless-ly but writes standard out to /dev/null to avoid IO errors
# See https://stackoverflow.com/questions/52376942/python-ioerror-errno-5-input-output-error
# or https://stackoverflow.com/questions/38238139/python-prevent-ioerror-errno-5-input-output-error-when-running-without-stdo/38238281#38238281
python3 main.py -v >/dev/null &

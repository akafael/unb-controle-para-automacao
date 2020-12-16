#!/bin/sh

prjdir=$(dirname $0)

gcc -Wall -I$prjdir/inc $prjdir/src/* -o $prjdir/bin/state_machine_fw


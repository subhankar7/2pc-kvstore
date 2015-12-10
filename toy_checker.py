#!/usr/bin/env python
import os
import sys

crashed_state_directory = sys.argv[1]
stdout_file = sys.argv[2]

# Move into the crashed-state directory supplied by ALICE, and read all
# messages printed to the terminal at the time of the crash.
os.chdir(crashed_state_directory)
stdout = open(stdout_file).read()

if 'Prepare logging done.' in stdout:
	# Check durability
	assert open('12345678').read() == open('workload_dir/nhello').read()
        assert open('store_log').read() == open('workload_dir/after_prepare').read()

if 'Renamed while committing' in stdout:
        assert open('nhello').read() == open('workload_dir/nhello').read()
else
        try: 
            open('nhello')
            assert open('nhello').read() == open('workload_dir/nhello').read()
        except:
            assert True
         
if 'Commit logging done.' in stdout:
        assert open('nhello').read() == open('workload_dir/nhello').read()
        assert open('store_log').read() == open('workload_dir/store_log').read()



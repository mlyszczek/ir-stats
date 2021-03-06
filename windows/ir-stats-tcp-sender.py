#!python3
# a lot of code stolen from
# https://github.com/kutu/pyirsdk/blob/master/tutorials/03%20Base%20application.md
#
# MIT licensed like original file

import irsdk
import time
import tempfile
import socket
import os

server_ip = "10.1.1.1"
server_port = 7454

# this is our State class, with some helpful variables
class State:
	ir_connected = False
	last_car_setup_tick = -1
	tmpfile = None

def loop():
	# on each tick we freeze buffer with live telemetry
	# it is optional, but useful if you use vars like CarIdxXXX
	# this way you will have consistent data from those vars inside one tick
	# because sometimes while you retrieve one CarIdxXXX variable
	# another one in next line of code could change
	# to the next iracing internal tick_count
	# and you will get incosistent data
	ir.freeze_var_buffer_latest()

	# dump iracing telemtry to a tmp file
	ir.parse_to(state.tmpfile)

	fsize = os.path.getsize(state.tmpfile)
	fd = open(state.tmpfile, "rb")
	l = fd.read(1024)

	# send fixed-size header to server, to easily
	# know when file ends
	msg = "IR_STATS_SOF: " + str(fsize).zfill(9) + "\n"
	serversock.send(msg.encode())
	# send file until everything is sent
	while True:
		serversock.send(l)
		l = fd.read(1024)
		if not l:
			break
	fd.close()

# here we check if we are connected to iracing
# so we can retrieve some data
def check_iracing():
	if state.ir_connected and not (ir.is_initialized and ir.is_connected):
		state.ir_connected = False
		# don't forget to reset your State variables
		state.last_car_setup_tick = -1
		# we are shutting down ir library (clearing all internal variables)
		ir.shutdown()
		print('irsdk disconnected')
	elif not state.ir_connected and ir.startup() and ir.is_initialized and ir.is_connected:
		state.ir_connected = True
		print('irsdk connected')

if __name__ == '__main__':
	# initializing ir and state
	ir = irsdk.IRSDK()
	state = State()
	fd, state.tmpfile = tempfile.mkstemp()
	print("connecting to the server")
	serversock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	serversock.connect((server_ip, server_port))
	print("connected to the server")

	try:
		# infinite loop
		while True:
			# check if we are connected to iracing
			check_iracing()
			# if we are, then process data
			if state.ir_connected:
				loop()
			# maximum you can use is 1/60
			# cause iracing updates data with 60 fps
			time.sleep(1/60)

	except KeyboardInterrupt:
		# press ctrl+c to exit
		pass

	serversock.close()
	state.tmpfile.close()

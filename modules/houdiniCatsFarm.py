import os
import re
from sys import argv

#-----------------------------------------------------------
hipfile = argv[1]
out_node = argv[2]
first_frame = int(argv[3])
last_frame = int(argv[4])
#-----------------------------------------------------------

#-----------------------------------------------------------
hou.hipFile.load(hipfile)
#----------------------------------------------------

if str(hou.node(out_node).type())=="<hou.NodeType for Driver arnold>":dopType=False
else:dopType=True

if dopType:

	# obtiene la ruta del dop network
	rop_output = hou.node(out_node)
	for node in rop_output.parent().children():
		try:
			dop_path = node.evalParm("doppath")
			break
		except:None
	#-----------------------------------------

	node_output = rop_output.evalParm("sopoutput")

	dirname=os.path.dirname(node_output)
	basename_full=os.path.basename(node_output)
	padding=re.findall('\d+', basename_full)[-1]
	basename=basename_full.split(padding)[0]

	ini=dirname+"/initialstate"
	if not os.path.isdir(ini):
		os.mkdir(ini)

	# chekea si el .sim anterior existe
	init_frame=ini+"/"+basename+str(first_frame-1)+".sim"
	if not os.path.isfile(init_frame):
		init_frame=None
	#-----------------------------------------
	if not first_frame==1:
		hou.node(dop_path).parm("startframe").set(first_frame-1)
	if init_frame:
		hou.node(dop_path).parm("initialstate").set(init_frame)

	if init_frame or first_frame==1:

		rop_output.render(frame_range=(first_frame,last_frame), verbose=True)

		dop_output = hou.node('/out').createNode('dop')
		dop_output.parm("doppath").set(dop_path)
		sim_file=ini+"/"+basename+str(last_frame)+".sim"
		dop_output.parm("dopoutput").set(sim_file)

		dop_output.render(frame_range=(last_frame,last_frame), verbose=False)

	else:print("\nNo simulation starting point was found.\n")

else:
	hou.node(out_node).render(frame_range=(first_frame,last_frame), verbose=True)


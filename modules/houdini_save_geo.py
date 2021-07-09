import sys

project = sys.argv[1]
node_path = sys.argv[2]
first_frame = int(sys.argv[3])
last_frame = int(sys.argv[4])

hou.hipFile.load(project)

file_cache_node = hou.node(node_path)

file_cache_node.parm('f1').deleteAllKeyframes()
file_cache_node.parm('f2').deleteAllKeyframes()

file_cache_node.parm('f1').set(first_frame)
file_cache_node.parm('f2').set(last_frame)

file_cache_node.parm("execute").pressButton()

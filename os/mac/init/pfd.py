import os

path=os.path.dirname(os.path.abspath(__file__))


pfconf='''block in all user root

set skip on lo0

tcp_pass="{ 22 137 138 139 445 7773 7774}"
udp_pass="{ 137 138 139 445 }"

pass in proto tcp to any port $tcp_pass keep state
pass out proto tcp to any port $tcp_pass keep state

pass in proto udp to any port $udp_pass keep state
pass out proto udp to any port $udp_pass keep state
'''


f=open("/etc/pf.conf","w")
f.write(pfconf)
f.close()


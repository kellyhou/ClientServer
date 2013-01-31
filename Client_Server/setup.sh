vagrant up
ssh vagrant@127.0.0.1 -p 2222 -i c:/Users/Kelly/.vagrant.d/insecure_private_key <<-EOF
	sudo apt-get -y install make gdb
EOF


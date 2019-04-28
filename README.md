# Reliable-UDP---Computer-Networks

This project implements UDP protocol allowing a user to transfer an audio file over UDP using Linux/GNU C socket. It implements reliability over the UDP by:
 
•	Adding sequence numbers to the packets 
•	Retransmission of lost packets (selective repeat) 
•	Re ordering of packets on the receiver side 
•	A window size of 5 UDP segments 

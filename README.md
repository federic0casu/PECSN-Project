# PECSN - Project
<h2>Opportunistic Cellular Network</h2>
<h3>Brief Description</h3>
<p>
   A cellular network transmits its traffic to <b>n</b> users. Each user has its own FIFO queue on the transmitting antenna. On each timeslot, users report to then
   antenna a <b>C</b>hannel <b>Q</b>uality <b>I</b>ndicator (<b>CQI</b>), i.e. a number from 1 to 15, which determines the number of bytes that the antenna can pack
   into a <b>R</b>esource <b>B</b>lock (<b>RB</b>). Then the antenna composes a frame of 25 RBs by scheduling traffic from the users, and sends the frame to the users. A
   packet that cannot be transmitted entirely will not be scheduled. An RB can only carry traffic for one user. However, two or more packets for the same user can share
   the same RB. (e.g., packet 1 is 1.5 RBs and packet 2 is 1.3 RBs, hence ceiling(1.5 + 1.3) = 3 RBs are required to transmit them).
</p>
<p>
   The antenna serves its users using an <b>opportunistic policy</b>: backlogged users are served by decreasing CQI. When a user is considered for service, its queue is
   emptied, if the number of unallocated RBs is large enough.
</p>
<h3>Project Objectives</h3>
<p>
   Consider the following workload for all users:
   <ul>
      <li>Packet interarrival times are IID RVs.</li>
      <li>Service demands (in bytes) are IID RV.</li>
   </ul>
   Study the <b>throughput</b> and <b>response time</b> of the above system with a varying workload.
</p>

# battleships-in-c
Battleships game in C
It probably sucks, I don't remember. Taken from report.txt:
> I made everything else except the missiles, since I didn't have time for them.
>
> I chose to do this project using a client-server approach, so it would be easy
to make multiplayer, and I think it looks clean.
> main.c initializes the server and the client, and handles the main menu.
server.c contains all the server-side function, and client.c is responsible
for asking for input and printing the board.
>
> I decided to store the ships in an array, so the server could easily check if
something was a hit. One dimension is for each ship, the second is for each
block of the ship, and the third is for the x,y coordinates and if the block
has been hit.
>
> To use the conditional compilation, define MONO or DEBUG in configuration.h
>
> Also, most inputs are validated, like for example so the ships dont go outside
the board

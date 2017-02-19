# SERVER

accept : client login
recv : client action

variable

>channel[20]
>room[20]

>client(ch 00, room 00)

server's channel, room vector

>pushback(cli-ch or cli-room)
>remove(cli-ch or cli-room)

# CLIENT

- 1. login
   - input name
- 2. choose channel [1~20]
- 3. Lobby[ch]
   - go to choose channel
   - go to room
   - chat in Lobby
- 4. Room[1~20]
   - go to Lobby
   - chat in Room

>set name : _#00** (**) name
>choose channel : _#01** (**) channel

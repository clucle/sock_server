# SERVER

accept : client login
recv : client action

variable

>channel[20]
>room[20]


# CLIENT

- 1. login
- 2. choose channel [1~20]
- 3. Lobby[ch]
   - go to choose channel
   - go to room
   - chat in Lobby
- 4. Room[1~20]
   - go to Lobby
   - chat in Room
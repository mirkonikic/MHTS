# MHTSv5

This project is finished, i will post code soon

**Komande:** <br />
**/hide** - hides chat in terminal<br />
**/show** - unhides chat in terminal<br />
**/msg0** - no notification<br />
**/msg1** - with gui notification<br />
**/msg2** - with sound notifications<br />
**/exit** - disconnnect<br />
**/clear** - clear screen same as clear in terminal<br />
**/scan** - scan who is online<br />
`/poke <User>` - same as facebook<br />
`/img [PATH]` - send image and feh/display when downloaded<br />
`/file [PATH]` - send file and store it when recieved<br />
`/nc_msg ""` - send the message between quotes only to those who dont use this app

Tutorial on how to install it:<br />
1.Create mhtc directory/folder somewhere in your filesystem<br />
2.CD into that folder<br />
3.Type in: git clone https://github.com/mirkonikic/MHTS<br />
4.When new version comes out, or if the one you have wont start just type:<br />
git pull origin master<br />

>`Uskoro:`
- [x] Add XOR i ROT13 enc.
- [ ] Add AES enc.
- [x] Make up your own encryption.
- [x] Fix /hide command
- [x] Add /scan (see who is online and when were they last active) 
- [ ] Add /poke (like fb command)
- [ ] Add /file send file drom your path and stores it to some folder
- [ ] Add /image to send image
- [ ] Add /nc_msg "" command for sending unencrypted messages to nc bots
- [ ] Publish mhtc on debian apt package?
- [ ] New login thread
- [ ] Make Android app that can communicate with this server
- [ ] Translate it to tor

`This was captured with network analysis after communicating with this app:`<br />
![](encrYpt.png)

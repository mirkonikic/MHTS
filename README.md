# MHTSv5

**Komande:** <br />
**/hide** - sakrije ti chat<br />
**/show** - ako je sakriven otkrije chat<br />
**/msg0** - bez notifikacija<br />
**/msg1** - sa gui notifikacijama<br />
**/msg2** - sa zvucnim notifikacijama<br />
**/exit** - diskonnekt<br />
**/clear** - obrisi ekran isto kao clear u terminalu<br />
**/scan** - skeniraj ko je online i kad je poslednji put bio<br />
`/poke <User>` - isto kao na fejsu<br />
`/img [PATH]` - posalji slikui display kad stigne<br />
`/file [PATH]` - posalji fajl i sacuvaj u folder kad stigne<br />
`/nc_msg ""` - posalji neku poruku iz navodnika onima sto nemaju tcpc a pokusavaju da pristupe

Da bi mogao da skidas ovaj apk iz terminala kad se apdejtuje, uradi ovo:<br />
1.Napravi folder negde u filesystemu<br />
2.Udji u taj folder sa cd<br />
3.Ukucaj: git clone https://github.com/mirkonikic/MHTS<br />
4.Sad samo sl put kad budes hteo da apdejtujes apk jer mi se promenio ip, samo idi u taj folder i ukucaj:<br />
git pull origin master<br />

ovde sam nasao ovo gore:<br />
https://docs.gitlab.com/ee/gitlab-basics/start-using-git.html<br />

>`Uskoro:`
- [x] Dodati XOR i ROT13 enc.
- [ ] Dodati AES enc.
- [x] Izmisli neku svoju enkripciju padded enc.
- [x] Popravi hide komandu
- [x] Dodati /scan (da vidis ko je online i posl put aktivan) 
- [ ] Dodati /poke (kao fejs mozda) komande
- [ ] Dodati /file da salje fajl sa nekog patha i cuva ga u folder
- [ ] Dodati /image da posalje sliku, display je i obrise
- [ ] Dodati /nc_msg "" da posalje iz navodnika ne enkriptovano nc botu
- [ ] Napraviti deb package na apt-get install mhtc?
- [ ] Nova nit za login

`Ovako izgleda poruka kad je enkriptovana:`<br />
![](encrYpt.png)

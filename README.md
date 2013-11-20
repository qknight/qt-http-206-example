# bugs
when i plug the network cable and use ./qt-http-206-example and then type 'q' and press RETURN it does not exit the program but
stays there for minutes.

## loosing network connection
when i unplug the network cable (host::eth0), the virtualbox guest can't detect it. this behaviour is not specific
to virtualization and affects all similar setups. for example if you unplug the network cable between your router to the switch
you have your test machine connected to.

 the virtualbox based setup i used:

   inet 
     |
     |   eth0|---------|   virtualized    |---------|
     \ ------|  host   |------------------|  guest  |
             |---------| vboxnet0    eth1 |---------|
                                       |
                                     inside the guest
                                      this interface is called eth1
                                      
             
this means, if i ping 8.8.8.8 from the guest, it will send these packages but they will end in nirvana:
 guest # ping 8.8.8.8
      PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
      From 192.168.56.101 icmp_seq=1 Destination Host Unreachable
      From 192.168.56.101 icmp_seq=2 Destination Host Unreachable
      From 192.168.56.101 icmp_seq=3 Destination Host Unreachable
      From 192.168.56.101 icmp_seq=4 Destination Host Unreachable
      From 192.168.56.101 icmp_seq=5 Destination Host Unreachable
      ...

 guest # time ./qt-http-206-example
      'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads 
      starting download with  7713  bytes 
      sleeping for  3907  ms 
      >> this example will download something defined in NetGet.cpp (if network is working) << 
      type the letter 'q' and hit return if you want to cancel the download and exit instantly without waiting for the download to finish 
      q
      you typed:  "q" 
      input handling exited 
      NetGet::cancelDownload() called 
      replyFinished handler called (this means QNetworkAccessManager was able to do something ;-) 
      error on download 
      
      canceling request  0 
      'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads 
      ~NetGet 
      ./qt-http-206-example  0.04s user 0.03s system 0% cpu 40.125 total


## ip a
3: eth1: <NO-CARRIER,BROADCAST,MULTICAST,UP> mtu 1500 qdisc pfifo_fast state DOWN qlen 1000
    link/ether 08:00:27:09:8c:39 brd ff:ff:ff:ff:ff:ff
    inet 192.168.56.101/24 brd 192.168.56.255 scope global eth1
    inet6 fe80::a00:27ff:fe09:8c39/64 scope link 
       valid_lft forever preferred_lft forever

## ip r 
default via 192.168.56.1 dev eth1 
169.254.0.0/16 dev eth0  proto kernel  scope link  src 169.254.237.33 
192.168.56.0/24 dev eth1  proto kernel  scope link  src 192.168.56.101 
192.168.122.0/24 dev virbr0  proto kernel  scope link  src 192.168.122.1 

someone else had the same discovery here:
  https://qt-project.org/forums/viewthread/11763
  http://www.qtcentre.org/threads/41649-QNetworkAccessManager-smart-handling-of-timeouts-network-errors
  
## what causes the stall?
i think the conclusions from the both threads are wrong. this is a DNS timeout with 40 seconds and i can reproduce this by
modifying /etc/resolv.conf to, 'nameserver 127.0.0.1' so that the connection gets a immediate timeout instead of waiting
forever. however, the ping from above suggests that it detects a timeout very fast. 

the cause is probably the QHostInfo DNS resolver routine:
    int info = QHostInfo::lookupHost("www.qt-project.org", this, SLOT(printResults(QHostInfo)));
    //QHostInfo::abortHostLookup ( info );
    
./qhostinfo-test
0.01s user 0.01s system 0% cpu 40.057 total

the same program with the abortHostLookup(..) call, terminates instantly.

## solution for my issue
the QNetworkAccessManager makes use of QHostInfo and when the destructor of it is called it must call abortHostLookup(..) for all
related objects so that shutdown does not wait for the 40 seconds timeout.


qt source on nix9000:~/qt-everywhere-opensource-src-4.8.5/src/network/access 

##  i see this behavour
'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads 
starting download with  7713  bytes 
sleeping for  3907  ms 
>> this example will download something defined in NetGet.cpp (if network is working) << 
type the letter 'q' and hit return if you want to cancel the download and exit instantly without waiting for the download to finish 
q
you typed:  "q" 
input handling exited 
NetGet::cancelDownload() called 
replyFinished handler called (this means QNetworkAccessManager was able to do something ;-) 
error on download 
 
canceling request  0 
'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads 
~NetGet 
(the program will stay in this stage forever and won't terminate)

## this is also discussed as a qt bug, see more references here
 - https://bugreports.qt-project.org/browse/QTBUG-33391
 - https://qt-project.org/forums/viewthread/11763
 - http://www.qtcentre.org/threads/41649-QNetworkAccessManager-smart-handling-of-timeouts-network-errors/page2


# what is this
qt-http-206-example is a little example featuring how to do partial content 206 http downloads using QNetworkAccessManager
this code is a bit hacky, so pick what you need.


# running the code
./qt-http-206-example

'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads 
starting download with  7713  bytes 
sleeping for  3907  ms 
>> this example will download something defined in NetGet.cpp (if network is working) << 
type the letter 'q' and hit return if you want to cancel the download and exit instantly without waiting for the download to finish 
replyFinished handler called (this means QNetworkAccessManager was able to do something ;-) 
Content status code:  QVariant(int, 302) 
Content type       :  QVariant(QString, "text/html; charset=iso-8859-1") 
Content size       :  QVariant(qlonglong, 384) 
============== Content begin: ============== 
"3c21444f43545950452048544d4c205055424c494320222d2f2f494554462f2f4454442048544d4c20322e302f2f454e223e0a3c68746d6c3e3c686561643e0a3c7469746c653e33303220466f756e643c2f7469746c653e0a3c2f686561643e3c626f64793e0a3c68313e466f756e643c2f68313e0a3c703e54686520646f63756d656e7420686173206d6f766564203c6120687265663d22687474703a2f2f616c667265642e71742d70726f6a6563742e6f72672f71742f6f6666696369616c5f72656c65617365732f6f6e6c696e655f696e7374616c6c6572732f71742d6c696e75782d6f70656e736f757263652d312e342e302d7838365f36342d6f6e6c696e652e72756e223e686572653c2f613e2e3c2f703e0a3c68723e0a3c616464726573733e4170616368652f322e322e3135202843656e744f53292053657276657220617420646f776e6c6f61642e71742d70726f6a6563742e6f726720506f72742038303c2f616464726573733e0a3c2f626f64793e3c2f68746d6c3e0a" 
============== Content end: ============== 
'q', then RETURN or CTRL+C to exit immediately; 'c' then RETURN to cancel further downloads 
q
you typed:  "q" 
NetGet::cancelDownload() called 


# license
this codebase is licensed - WTFPL – Do What the Fuck You Want to Public License -> http://www.wtfpl.net/




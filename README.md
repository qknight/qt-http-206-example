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



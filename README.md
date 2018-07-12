# Robosapien Wi-Fi

<b>Modyfikacja robota</b>

Modyfikacja robota polegała na dodaniu do niego układu WeMos D1 mini, którego kompaktowe wymiary pozwalają na schowanie go wewnątrz obudowy robota. Oryginalne sterowanie Robosapien działało w oparciu o podczerwień, więc najprostszym sposobem na modyfikację, było wpięcie się do przewodu prowadzącego z odbiornika podczerwieni do układu logiki robota i symulowanie wysyłanych przez pilota sygnałów. Inspiracją do modyfikacji robota było rozwiązanie <a href="http://playground.arduino.cc/Main/RoboSapienIR" target="_blank">RoboSapienIR</a> w serwisie The Arduino Playground. To stąd zaczerpnąłem najważniejszą funkcję odpowiedzialną za symulowanie sygnałów przesyłanych przez pilota. Jako, że wzorowałem się na tym rozwiązaniu, mój kod może być w wielu miejscach bardzo podobny.

W Arduino wykorzystałem dwa piny, GPIO5 jako wejście sygnału z odbiornika podczerwieni i GPIO4 jako wyjście sygnału do układu robota. Arduino dzięki prostej funkcji przesyła właściwy kod do robota jakby pochodził on z pilota. Samo Arduino tuż po uruchomieniu łączy się z jedną z zaprogramowanych sieci Wi-Fi. Po połączeniu Arduino korzystając ze stworzonej dla robota usługi sieciowej (web service) wysyła swój adres IP, który został mu przydzielony oraz SSID sieci, z którą się połączył. Usługa sieciowa napisana w PHP działa w oparciu o wzorzec REST i zapisuje otrzymane dane do bazy MySQL, aby później aplikacja sterująca na smartfona mogła te dane pobrać i we właściwy sposób wykorzystać. W następnym kroku Arduino sam tworzy prostą usługę sieciową i od tego momentu oczekuje na komendy przesyłane mu za pomocą protokołu REST.

Układy logiki w Robosapien działają z napięciem 3,3V, które nie jest w stanie zasilić jednocześnie układu Arduino i mechanizmów robota, nic w tym momencie nie działa prawidłowo. Układ Arduino zasiliłem więc dwiema bateriami typu LR03. Nie chcąc robić otworów na dodatkowy włącznik dla Arduino wykorzystałem moduł wykonawczy z tranzystorem MOSFET IRF520. Zasilanie i sygnał sterujący modułem pobrałem z włącznika, masę wziąłem z przewodu wychodzącego ze złącza w górnej części układu robota. Od teraz włączając robota, ten zasila moduł MOSFET, który z kolei steruje zasilaniem Arduino. Układ D1 mini umieściłem obok szyi robota,  a pod przednim pancerzem jest wystarczająco przestrzeni, aby zmieścić jednocześnie moduł MOSFET oraz dwie baterie zasilające. 

<b>Sterowanie robotem</b>

Aby sterować zmodyfikowanym Robosapien wystarczy znać jego adres IP, który został przyznany robotowi przez router oraz być połączonym do tej samej sieci Wi-Fi. Do samego sterowania nie jest wymagana jakakolwiek aplikacja, same polecenia do robota są przesyłane za pomocą protokołu REST. Znając jego adres i składnię komend, przy pomocy dowolnej strony oferującej tworzenie zapytań REST możemy wysłać polecenia do robota. Linia komend została podzielona na cztery fragmenty: body, left, right i program, a każdy z nich odpowiada innemu rodzajowi czynności i obsługuje sobie właściwe komendy.

I tak „body” odnosi się do ciała, mamy tam: walkforward, walkbackward, turnleft, turnright oraz stop.

Części „left” i „right” to ramiona robota obsługujące komendy: armup, armdown, armout, armin, handthump, handthrow, handpickup, handsweep, handstrike1, handstrike2 oraz handstrike3.

Natomiast „program” odnosi się do poleceń ogólnych: poweroff, sleep, wakeup, burp, high5, bulldozer, oops, whistle, talkback, roar, alldemo, karateskits, rudeskits i dance.

Przykładowo:

POST http://192.168.1.101/body/walkforward

POST http://192.168.1.101/left/armup 

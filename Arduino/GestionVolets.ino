
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

int tempo = 250;
int up = 5;    // Wemos D1
int down = 4;  // Wemos D2


// Creation d un serveur qui ecoute sur le port 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Il faut connecter le wemos au wifi
  
  //if you get here you have connected to the WiFi
  Serial.println("ESP : connecté");

  // Declaration port "down" en sortie
  pinMode(down, OUTPUT);
  digitalWrite(down, LOW);

  // Declaration port "up" en sortie
  pinMode(up, OUTPUT);
  digitalWrite(up, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // demarrage du serveur
  server.begin();
  Serial.println("ESP : Server HTTP OK");

  // Adresse IP locale attribuee
  Serial.print("ESP : IP : ");
  Serial.println(WiFi.localIP());
}


void loop() {
  // On attend la connexion d'un client
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  client.print(pageHTML());

  // On attend les donnees envoyees par un client
  Serial.println("Nouveau client");
  while (!client.available()) {
    delay(1);
  }

  // Lecture premiere ligne de la requete
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  //----- FERMER -----
  if (req.indexOf("/FERMER") != -1) 
  {
    Serial.println("Demande fermeture volet");

    // Positionnement port "down"
    digitalWrite(down, 1);                   // Début appui pour fermeture volet
    delay(tempo);
    digitalWrite(down, 0);                    // Fin appui pour fermeture volet


    client.print(reponseHTML());
    // give the web browser time to receive the data
    delay(1);
    
    blink();
   
    Serial.println("client déconnecté");
  }

  //----- OUVRIR -----
  else if (req.indexOf("/OUVRIR") != -1)
  {
    Serial.println("Demande ouverture volet");
    
    // Positionnement port "up"
    digitalWrite(up, 1);                   // Début appui pour ouverture volet
    delay(tempo);                          
    digitalWrite(up, 0);                    // Fin appui pour ouverture volet


    client.print(reponseHTML());
    // give the web browser time to receive the data
    delay(1);

    blink();
 
    Serial.println("client déconnecté");
  }
 
}

String reponseHTML()
{
  String valeur="<br>ok";
  
  return valeur;
}

String pageHTML()
{
  String html="HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  html+="<br><input type=\"button\" name=\"b1\" value=\"Ouvrir\" onclick=\"location.href='/OUVRIR'\">";
  html+="<br>";
  html+="<br><input type=\"button\" name=\"b1\" value=\"Fermer\" onclick=\"location.href='/FERMER'\">";
  html+="</html>\n";
  html+="<br><br>";
  
  return html;
}

void blink()
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
}

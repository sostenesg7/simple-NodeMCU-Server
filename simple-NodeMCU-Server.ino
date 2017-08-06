#include <ESP8266WiFi.h>

#define led1 5
#define led2 2

const char *ssid = "SSOUZA";
const char *password = "1234512345";

IPAddress ip(192, 168, 0, 120);

//IP do roteador da sua rede wifi
IPAddress gateway(192, 168, 0, 1);

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

void setup()
{
	Serial.begin(9600);
	delay(10);

	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);

	// Connect to WiFi network
	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.config(ip, gateway, subnet);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.println("WiFi Conectado");

	// Start the server
	server.begin();
	Serial.println("Servidor Iniciado");

	// Print the IP address
	Serial.println(WiFi.localIP());
}

void loop()
{
	// Check if a client has connected
	WiFiClient client = server.available();
	if (!client)
	{
		return;
	}

	// Wait until the client sends some data
	Serial.println("Novo Cliente");
	while (!client.available())
	{
		delay(1);
	}

	// Read the first line of the request
	String request = client.readStringUntil('\r');
	Serial.println(request);
	client.flush();

	if (request.indexOf("led1") != -1)
	{
		digitalWrite(led1, !digitalRead(led1));
	}
	if (request.indexOf("led2") != -1)
	{
		digitalWrite(led2, !digitalRead(led2));
	}
	if (request.indexOf("todos") != -1)
	{
		digitalWrite(led1, !digitalRead(led1));
		digitalWrite(led2, !digitalRead(led2));
	}

	int luminosidade = analogRead(A0);

	//client.println("http/1.1 200 OK");
	//client.println("Content-Type: text/html");
	//client.println("");

	String retorno = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";

	if (digitalRead(led1))
	{
		//client.print("l1on");
		retorno += "l1on";
	}
	else
	{
		//client.print("l1off");
		retorno += "l1off";
	}
	//client.println(",");
	retorno += ",";

	if (digitalRead(led2))
	{
		//client.print("l2on");
		retorno += "l2on";
	}
	else
	{
		//client.print("l2off");
		retorno += "l2off";
	}
	//client.println(",");
	retorno += ",";
	//client.print(luminosidade);
	retorno += luminosidade;

	retorno += "</html>\n";
	client.print(retorno);
	delay(1);
	Serial.println("Client disonnected");
}
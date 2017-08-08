#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define led1 5
#define led2 2

const char *ssid = "SSOUZA";
const char *password = "1234512345";

IPAddress ip(192, 168, 0, 120);

//IP do roteador da sua rede wifi
IPAddress gateway(192, 168, 0, 1);

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

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

	//Espefifica qual ação realizar, dependendo da rota
	//Nesse caso, a rota base é o próprio ip, ex: http://192.168.0.1/
	server.on("/", handleNewClient);
	// Start the server
	server.begin();
	Serial.println("Servidor Iniciado");
	// Print the IP address
	Serial.println(WiFi.localIP());
}

void loop()
{
	server.handleClient();
}

void handleNewClient()
{
	Serial.println("Novo cliente Conectado");
	//Exibe todos os argumentos, só para visualizar em caso de testes
	String argumento;
	for (int i = 0; i < server.args(); i++)
	{
		argumento = "[" + (String)i + "]=" + server.argName(i);
		Serial.println(argumento);
	}

	//Trata os parametros passados por quem fez a requisicao

	//Por exemplo, um GET: http://192.168.0.120/?led1=on&led2=off
	if (server.arg("led1") != "")
	{
		//acao aqui
		digitalWrite(led1, !digitalRead(led1));
	}
	if (server.arg("led2") != "")
	{
		//acao aqui
		digitalWrite(led2, !digitalRead(led2));
	}
	if (server.arg("todos") != "")
	{
		//acao aqui
		digitalWrite(led1, !digitalRead(led1));
		digitalWrite(led2, !digitalRead(led2));
	}

	int luminosidade = analogRead(A0);

	String json = "{";   //Inicio do JSON
	json += "\"led1\":"; //Campo led1
	if (digitalRead(led1))
	{
		json += "\"on\"";
	}
	else
	{
		json += "\"off\"";
	}
	json += ",";
	json += "\"led2\":"; //Campo led2
	if (digitalRead(led2))
	{
		json += "\"on\"";
	}
	else
	{
		json += "\"off\"";
	}
	json += ",";
	json += "\"luminosidade\":"; //Campo luminosidade
	json += luminosidade;
	json += "}"; //Fim do json

	//	client.print(json);
	//envia os dados para quem fez a requisicao
	server.send(200, "application/json", json);
	Serial.println("Client disconnected");
}
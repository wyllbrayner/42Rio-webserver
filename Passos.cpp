/*
Compartilhando o que estou fazendo sobre o webserver:
Alteração na estrutura de pastas:
	O config passará para a pasta "/etc/webserver/sites-available"
		-> similar a pasta /etc/nginx/sites-available do nginx.
	Os arquivos das páginas passarão para a pasta "var/www/html"
		-> similar a estrutura de pastas do nginx.

Alteração no config:
	=> O campo "root" passará a especificar o caminho já considerando o caminho "var/www/html".Assim, se o campo root possuir apenas "/", significa que o restante dos arquivos/diretórios deste servidor serão pesquisados a partir da pasta "var/www/html". Da mesma forma, caso o campo root possua "meusite", significa que o restante dos arquivos/diretórios deste servidor serão pesquisados a partir da pasta "var/www/html/meusite" e assim por diante (STATUS: FINALIZADO).
	OBS: caso o servidor não preencha este campo colocarei o "/" na definição deste campo.
	=> Inclusão do campo "max_body_size" contendo um inteiro que determina o tamanho máximo do corpo da requisição/resposta (STATUS: FINALIZADO).
	OBS: vamos pensar em quantidade de caracteres permitidos para facilitar.
	=> Inclusão dos campos index (vetor de string contendo os arquivos permitidos em cada local) e autoindex (um booleano informando se o local permite autoindex) em cada location. Já existia o campo allow_methods informando quais métodos são permitidos em cada pasta (STATUS: FINALIZADO).
	=> Inclusão de um campo map "error_page" para definição de um caminho para erros. Estes erros devem estar na estrutura de pastas definida no root, mas caso não esteja vou definir que o servidor utilizará o padrão do servidor (STATUS: FINALIZADO).
	OBS: caso o servidor não preencha este campo colocarei "default" na definição deste campo para utilização de erros padrões do servidor (STATUS: FINALIZADO).
	=> Inclusão do campo "return" no location (STATUS: FINALIZADO).
	=> Inclusão de um servidor padrão caso nenhum arquivo de configuração seja enviado (STATUS: FINALIZADO).
Print dos servidores:
	=> Inclusão de métodos para printar as informações de cada servidor carregado (STATUS: FINALIZADO).
Listen padrão
	=> Caso o servidor não possua um camppo listen prenchido, colocar uma porta padrão (ex: 4242;) (STATUS: FINALIZADO).
	=> Correção de bug que lançava exceção caso não existisse um location (STATUS: FINALIZADO).
	=> ajuste na configuração do method em location que coloca "GET", "DELETE" e "POST" caso nenhum método esteja presento no location (STATUS: FINALIZADO).
	=> inclusão do método buildSingleServer(void) em ParserServer de tal forma que cada servidor possua apenas uma Porta (STATUS: FINALIZADO).
Return no config:
	=> verificar se o location possui return diferente do path e, para aqueles cujos campos forem diferentes, procurar o path correspondente ao return e substiruir as informações até que identifiquemos o último path a ser retornado (STATUS: FINALIZADO).
Validação do servidor:
	=> Verificar se o config possui mais de um servidor e se eles podem ser considerados "iguais". Vou olhar a semelhança das portas, root, servername e host. Se forem iguais "apenas" ignoro na inicialização. (STATUS: FINALIZADO)
	OBS: Como não iniciei, podem sugerir interpretações.
Inicialização do Socket
	=> Fazer o socket considerar o ip definido ao servidor (STATUS: FINALIZADO).
	=> Verificar se servidores escutam a mesma porta. (STATUS: FINALIZADO)
	OBS: Ainda possuo dúvidas quanto ao tratamento a ser dado. Se apenas o primeiro realmente escutará a porta. Ou outro comportamento.

Erro_Page:
	Montar as páginas de erro padrão (STATUS: FINALIZADO)

Alterar o método fileRequested(da classe Client)
	Minha ideia é que ele possua uma espécie de "seletor" que identifique o arquivo a ser retornado. Ele já faz tal função, porém precisa incluir mais coisas com os ajustes que restam implementar. (STATUS: FINALIZADO)

Auto_index:
	Montar o auto index. (STATUS: FINALIZADO)

Delete:
	Ajustar o código do delete. (STATUS: FINALIZADO)

Método Não autorizado:
	Implementar o método não autorizado no servidor. (STATUS: FINALIZADO)
*/

/*
Pendência!!!!!!
após terminar de ler o body (corpo) da request, verificar se o tamanho do corpo supera o max size body permitido pelo servidor.
Caso supere, enviar error 413?
caso contrário, seguir com processo de leitura para envio ao cliente.  

*/

std::map<std::string, std::string>		Request::initialize_mime_types()
{
    std::map<std::string, std::string> mime_types;

	mime_types[".aac"]      = "audio/aac";
	mime_types[".abw"]      = "application/x-abiword";
	mime_types[".arc"]      = "application/octet-stream";
	mime_types[".avi"]      = "video/x-msvideo";
	mime_types[".azw"]      = "application/vnd.amazon.ebook";
	mime_types[".bin"]      = "application/octet-stream";
	mime_types[".bz"]       = "application/x-bzip";
	mime_types[".bz2"]      = "application/x-bzip2";
	mime_types[".csh"]      = "application/x-csh";
	mime_types[".css"]      = "text/css";
	mime_types[".csv"]      = "text/csv";
	mime_types[".doc"]      = "application/msword";
	mime_types[".epub"]     = "application/epub+zip";
	mime_types[".gif"]      = "image/gif";
	mime_types[".htm"]      = "text/html";
	mime_types[".html"]     = "text/html";
	mime_types[".ico"]      = "image/x-icon";
	mime_types[".ics"]      = "text/calendar";
	mime_types[".jar"]      = "Temporary Redirect";
	mime_types[".jpeg"]     = "image/jpeg";
	mime_types[".jpg"]      = "image/jpeg";
	mime_types[".js"]       = "application/js";
	mime_types[".json"]     = "application/json";
	mime_types[".mid"]      = "audio/midi";
	mime_types[".midi"]     = "audio/midi";
	mime_types[".mpeg"]     = "video/mpeg";
	mime_types[".mpkg"]     = "application/vnd.apple.installer+xml";
	mime_types[".odp"]      = "application/vnd.oasis.opendocument.presentation";
	mime_types[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet";
	mime_types[".odt"]      = "application/vnd.oasis.opendocument.text";
	mime_types[".oga"]      = "audio/ogg";
	mime_types[".ogv"]      = "video/ogg";
	mime_types[".ogx"]      = "application/ogg";
	mime_types[".png"]      = "image/png";
	mime_types[".pdf"]      = "application/pdf";
	mime_types[".ppt"]      = "application/vnd.ms-powerpoint";
	mime_types[".rar"]      = "application/x-rar-compressed";
	mime_types[".rtf"]      = "application/rtf";
	mime_types[".sh"]       = "application/x-sh";
	mime_types[".svg"]      = "image/svg+xml";
	mime_types[".swf"]      = "application/x-shockwave-flash";
	mime_types[".tar"]      = "application/x-tar";
	mime_types[".tif"]      = "image/tiff";
	mime_types[".tiff"]     = "image/tiff";
	mime_types[".ttf"]      = "application/x-font-ttf";
	mime_types[".txt"]      = "text/plain";
	mime_types[".vsd"]      = "application/vnd.visio";
	mime_types[".wav"]      = "audio/x-wav";
	mime_types[".weba"]     = "audio/webm";
	mime_types[".webm"]     = "video/webm";
	mime_types[".webp"]     = "image/webp";
	mime_types[".woff"]     = "application/x-font-woff";
	mime_types[".xhtml"]    = "application/xhtml+xml";
	mime_types[".xls"]      = "application/vnd.ms-excel";
	mime_types[".xml"]      = "application/xml";
	mime_types[".xul"]      = "application/vnd.mozilla.xul+xml";
	mime_types[".zip"]      = "application/zip";
	mime_types[".3gp"]      = "video/3gpp audio/3gpp";
	mime_types[".3g2"]      = "video/3gpp2 audio/3gpp2";
	mime_types[".7z"]       = "application/x-7z-compressed";

	return mime_types;
}

# Informações Gerais
Este projeto integra o sexto ciclo de projetos da [42Rio](https://42.rio/).
Neste projeto devemos desenvolver um servidor web que replique as funcionalidades do [nginx](https://pt.wikipedia.org/wiki/Nginx). Este servidor deve ser desenvolvido utilizando, apenas, a **linguagem C++** (as instruções gerais deste projeto encontram-se no [en.subject.pdf](./en.subject.pdf) disponibilizado).
</br>
</br>

# Conhecimentos desenvolvidos durante este projeto
<ul>
    <li>
        Lógica de programação.
    </li>
    <li>
        Linguagem C++.
    </li>
    <li>
        Funcionamento do nginx como servidor web.
    </li>
    <li>
        HTTP Request/response.
    </li>
</ul>

# Como clonar o repositório...
    Utilize um sistema operacional baseado no Unix (Linux ou MacOs), clone o [projeto](https://github.com/wyllbrayner/42Rio-webserver) do **github**.
</br>
</br>

# Como clonar...
### A webserver.
Após clonar o repositório do [projeto](https://github.com/wyllbrayner/42Rio-webserver), entre na pasta do projeto e realize o comando **make** no terminal. Será criado um arquivo webserver no repositório.

# Como utilizar...
### A página padrão do webserver.
No terminal, coloque ./webserver [sem parâmetro] e no navegador, de sua preferência, acesse o endereço 127.42.42.42:4242. 

### A página de sua preferência.
No terminal, coloque ./webserver [path_do_arquivo_de_configuração_do_servidor.conf] e acesse o IP:PORTA definidos no arquivo de configuração.
ex: ./webserver etc/webserver/sites-available/default00.conf e acesse 127.0.0.1:8008 no seu navegador.
</br>
</br>

<p>Current solution proposes a C++ implementation of a web client sending requests to a public server using the <b>HTTP</b> protocol.</p>

<h4>Program flow</h4>

<p>Program continuously receives commands from standard input, until <i>exit</i> command is given. For every command, a new connection is opened and a request is sent to the server.

Depending on the needed functionality, it composes a <i><b>GET</i></b>, <i><b>POST</i></b> or <i><b>DELETE</i></b> request in order to obtain the necessary response.

<h4>Authorization process</h4>
<p>When logging in with a valid account (that is an account that was previously registered), the program receives from the server a <i>session cookie</i> which is memorized in order to indicate that the same session is taking place.</p>

<p>After a valid <i>enter_library</i> request, the server offers an authorization cookie, which will be memorized as well in order to demonstrate library access in the following requests.</p>

<p>When a user logs out, the token is invalidated in client program. </p>

<h4>Error handling</h4>
<p>Errors returned by the server are parsed and printed to the standard output. </p>

<p>Moreover, the client program validates the <i>username</i> and <i>password</i> fields to not contain any kind of whitespaces; it also reads <i>page_count</i> field as string and checks if it can be converted to an integer value; In case of misuse, program prints error message and does not send invalid http request.</p>

<h4>Data parsing</h4>
<p> Data is parsed in form of <i>json</i> strings/objects.</p>
<p> <b>Nlohmann</b> library was used in order to ease the parsing process.</p>
<p>
When information was added to the server, a new JSON object was created as first-class data type.<br>
When information was retrieved, the parser was invoked and the information was printed (note that in our case, information could have been printed without being parsed, but parsing suggests a more general approach of handling).</p>

<p>Nlohmann library made this part of the project very straightforward, as the value type does not need to be precised. Moreover, library provided function for casting from <i>string</i> to <i>char *</i>, making it easy to switch between these data types when needed.</p>


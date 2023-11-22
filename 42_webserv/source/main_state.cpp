enum ServerState {
    INITIALIZING,
    IDLE,
    PRE_PROCESSING_REQUEST,
    PROCESSING_REQUEST,
    SEND_RESPONSE,
    SHUT_DOWN
};


int main() {
    // VARIABLES AND STRUCT INITIALIZATION
    ServerState currentState = INITIALIZING;
    // ConfigFile config;

    while (42) {
        switch (currentState) {
            case INITIALIZING:
                // Start and configure the server.
                // Configure connection non-blocking.
                currentState = IDLE;
                break;
            case IDLE:
                // Wait for a connection.
                currentState = PRE_PROCESSING_REQUEST;
                break;
            case PRE_PROCESSING_REQUEST:
                // Read the HTTP request.
                // Parse the HTTP request.
                currentState = PROCESSING_REQUEST;
                break;
            case PROCESSING_REQUEST:
                // Process the request.
                // Send the request to the CGI.
                // Route the request.
                currentState = SEND_RESPONSE;
                break;
            case SEND_RESPONSE:
                // Create the HTTP response.
                // Send the response.
                currentState = IDLE;
                break;
            case SHUT_DOWN:
                // Close the server.
                // Close the connection.
                break;
        }
    }
    // free what is left.
    return 0;
}
#include <coap3/coap.h>
#include <stdio.h>

int main(void) {
    coap_startup();

    coap_context_t *ctx = coap_new_context(NULL);
    if (!ctx) {
        fprintf(stderr, "Error creating CoAP context\n");
        return -1;
    }

    // Setup the endpoint (WARNING: WILL BE BINDING TO ALL INTERFACES ON PORT 5683)
    coap_address_t serv_addr;
    coap_address_init(&serv_addr);
    serv_addr.addr.sin.sin_family = AF_INET;
    serv_addr.addr.sin.sin_addr.s_addr = INADDR_ANY;
    serv_addr.addr.sin.sin_port = htons(COAP_DEFAULT_PORT);

    coap_endpoint_t *endpoint = coap_new_endpoint(ctx, &serv_addr, COAP_PROTO_UDP);
    if (!endpoint) {
        fprintf(stderr, "ERROR CREATING CoAP ENDPOINT\n");
        coap_free_context(ctx);
        return -1;
    }

    // main loop
    while (1) {
        coap_io_process(ctx, COAP_IO_WAIT);
    }

    coap_free_context(ctx);
    coap_cleanup();
    return 0;
}

static void hnd_get_signal_state(coap_resource_t *resource,
                                 coap_session_t *session,
                                 const coap_pdu_t *request,
                                 const coap_string_t *query,
                                 coap_pdu_t *response) {
    // Build the payload: in this example we will be crafting and sending a traffic signal color
    // I will implement in JSON, but this is a big no no because it essentially negates the benefits of CoAP
    // I will later use CSMP encoding
    char payload[16] = "{\"state\": \"RED\"}";
    coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
    // Set content format
    unsigned char buf[3];
    coap_insert_option(response, COAP_OPTION_CONTENT_FORMAT, buf, coap_encode_var_safe(buf, sizeof(buf), COAP_MEDIATYPE_APPLICATION_JSON));

    coap_add_data(response, strlen(payload), (const uint8_t *)payload);
}
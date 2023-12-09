#include <coap3/coap.h>

int main() {
    coap_context_t *ctx;
    coap_address_t dst;
    coap_uri_t uri;
    coap_pdu_t *pdu;
    coap_optlist_t *optlist = NULL;
    coap_string_t *payload;

    coap_startup();

    // URI 파싱
    if (coap_split_uri((uint8_t *)"coap://192.168.1.100/resource", 27, &uri) < 0) {
        return -1;
    }

    // 목적지 주소 설정
    coap_address_init(&dst);
    dst.addr.sin.sin_family = AF_INET;
    dst.addr.sin.sin_port = htons(uri.port);
    inet_aton("192.168.1.100", &dst.addr.sin.sin_addr);

    // CoAP context 생성
    ctx = coap_new_context(NULL);
    if (!ctx) {
        return -1;
    }

    // CoAP PDU 생성
    pdu = coap_pdu_create(COAP_MESSAGE_CON, COAP_REQUEST_GET, coap_new_message_id(ctx), COAP_MAX_PDU_SIZE);

    // 옵션 추가 (URI Path)
    coap_add_optlist_pdu(pdu, &optlist);

    // 요청 보내기
    coap_send(ctx, ctx->endpoint, &dst, pdu);

    // CoAP context 삭제
    coap_free_context(ctx);
    coap_cleanup();

    return 0;
}


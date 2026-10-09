#define TAM_PAGINA 4096
unsigned pagina = endereco / TAM_PAGINA; /* ou endereco >> 12 */
unsigned desloc = endereco % TAM_PAGINA; /* ou endereco & 0xFFF */


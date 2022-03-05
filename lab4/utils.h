int bytes_to_int(uint8_t chr[4], bool is_new_version_mode);

void int_to_bytes(
        int x,
        uint8_t *ch,
        bool is_new_version_mode,
        TagHeader header
);

void print_frame(TagFrame frame, char *temp);
char* read_data(int frame_size, FILE *file);

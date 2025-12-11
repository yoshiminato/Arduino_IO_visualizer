/* 提出ファイルを読み込んでプログラム部分を抽出し、実行可能なcファイルに編集してファイル書き出ししたい */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *filename = "./data/42竹内良輔/課題2_1_2425050428.docx";
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "python3 ./scripts/convert_docx2txt.py --input \"%s\"", filename);



    FILE *fp = popen(cmd, "r");

    FILE *execute_src_file = fopen("./src/execute_file.c", "w");
    fprintf(execute_src_file, "#include \"override.h\"\n");
    fprintf(execute_src_file, "#include \"pin_state.h\"\n");
    fprintf(execute_src_file, "#include \"error.h\"\n");
    fprintf(execute_src_file, "#include <stdio.h>\n");

    fprintf(execute_src_file, "\n");
    fprintf(execute_src_file, "double scale = 1.0 / 100000;\n");
    fprintf(execute_src_file, "void setup();\n");
    fprintf(execute_src_file, "void loop();\n");
    fprintf(execute_src_file, "\n");    
    fprintf(execute_src_file, "int main(int argc, char *argv[]) {\n");
    
    fprintf(execute_src_file, "    init();\n");
    fprintf(execute_src_file, "    setup();\n");
    fprintf(execute_src_file, "    if (argc > 1)\n");
    fprintf(execute_src_file, "        loadInputPinsStateFromFile(argv[1]);\n");
    fprintf(execute_src_file, "    while(!end_sim_flag) {\n");
    fprintf(execute_src_file, "        loop();\n");
    fprintf(execute_src_file, "    }\n");
    fprintf(execute_src_file, "    for(int i = 0; i < pin_count; i++) {\n");
    fprintf(execute_src_file, "        PinState* state = &pin_states[i];\n");
    fprintf(execute_src_file, "        int pin = state->number;\n");
    fprintf(execute_src_file, "        printf(\"%%-3d : \", pin);\n");
    fprintf(execute_src_file, "        for(int j = 0; j <= state->state_count; j++) {\n");
    fprintf(execute_src_file, "            State* s = &state->log[j];\n");
    fprintf(execute_src_file, "            int duration = s->duration;\n");
    fprintf(execute_src_file, "            double value = s->value;\n");
    fprintf(execute_src_file, "            if(s->value == 1)\n");
    fprintf(execute_src_file, "                for(int k=0; k<duration*scale; k++)\n");
    fprintf(execute_src_file, "                    printf(\"*\");\n");
    fprintf(execute_src_file, "            else \n");
    fprintf(execute_src_file, "                for(int k=0; k<duration*scale; k++)\n");
    fprintf(execute_src_file, "                    printf(\" \");\n");
    fprintf(execute_src_file, "        }\n");
    fprintf(execute_src_file, "    printf(\"\\n\");\n");
    fprintf(execute_src_file, "    }\n");

    fprintf(execute_src_file, "    return 0;\n");
    fprintf(execute_src_file, "}\n\n");

    if (!fp) {
        perror("popen");
        return 1;
    }

    char buf[1024];
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        fprintf(execute_src_file, "%s", buf);
    }

    pclose(fp);
    fclose(execute_src_file);
    return 0;
}
/* 提出ファイルを読み込んでプログラム部分を抽出し、実行可能なcファイルに編集してファイル書き出ししたい */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    
    /* arduinoプログラムの実行に必要な関数群の定義と波形描画に必要なスレッドの作成 */
    FILE *execute_src_file = fopen("./src/execute_file.c", "w");
    fprintf(execute_src_file, "#include \"override.h\"\n");
    fprintf(execute_src_file, "#include \"pin_state.h\"\n");
    fprintf(execute_src_file, "#include \"error.h\"\n");
    fprintf(execute_src_file, "#include \"draw.h\"\n");
    fprintf(execute_src_file, "#include \"queue.h\"\n");
    fprintf(execute_src_file, "#include \"input.h\"\n");
    fprintf(execute_src_file, "#include <stdio.h>\n");
    fprintf(execute_src_file, "#include <stdlib.h>\n");
    fprintf(execute_src_file, "#include <math.h>\n");
    fprintf(execute_src_file, "#include <pthread.h>\n");

    fprintf(execute_src_file, "\n");

    fprintf(execute_src_file, "/* Thread identifiers.  The structure of the attribute type is not exposed on purpose.  */\n");
    fprintf(execute_src_file, "typedef unsigned long int pthread_t;\n");

    fprintf(execute_src_file, "\n");

    fprintf(execute_src_file, "pthread_t draw_thread;\n");
    fprintf(execute_src_file, "pthread_t publish_thread;\n");
    fprintf(execute_src_file, "pthread_t input_thread;\n");

    fprintf(execute_src_file, "double scale = 1.0 / 100000;\n");
    fprintf(execute_src_file, "void setup();\n");
    fprintf(execute_src_file, "void loop();\n");
    fprintf(execute_src_file, "\n");    

    fprintf(execute_src_file, "int main(int argc, char *argv[]) {\n");
    fprintf(execute_src_file, "    end_simulation_time_us = 10 * pow(10, 7);\n");
    fprintf(execute_src_file, "    init();\n");
    fprintf(execute_src_file, "    setup();\n");
    fprintf(execute_src_file, "    if (argc > 1)\n");
    fprintf(execute_src_file, "        loadInputPinsStateFromFile(argv[1]);\n");
    fprintf(execute_src_file, "    if (argc == 3)\n");
    fprintf(execute_src_file, "        end_simulation_time_us = atoll(argv[2]) * pow(10, 6);\n");
    fprintf(execute_src_file, "        \n");

    fprintf(execute_src_file, "    for(int i = 0; i < MAX_PIN_NUM; i++) {\n");
    fprintf(execute_src_file, "        InitQueue(&dataBuffer[i]);\n");
    fprintf(execute_src_file, "    }\n");

    fprintf(execute_src_file, "    pthread_create(&draw_thread, NULL, draw, NULL);\n");
    fprintf(execute_src_file, "    pthread_create(&publish_thread, NULL, publishThreadFunction, NULL);\n");
    fprintf(execute_src_file, "    pthread_create(&input_thread, NULL, makeInputThread, NULL);\n");
    fprintf(execute_src_file, "    while(!end_sim_flag){\n");
    fprintf(execute_src_file, "        loop();\n");
    fprintf(execute_src_file, "        if(getSimulationTimeus() >= end_simulation_time_us){ \n");
    fprintf(execute_src_file, "            end_sim_flag = 1;\n");
    fprintf(execute_src_file, "            break;\n");
    fprintf(execute_src_file, "        }\n");
    fprintf(execute_src_file, "    }\n");
    fprintf(execute_src_file, "    cleanupSimulation();\n");

    fprintf(execute_src_file, "    pthread_join(draw_thread, NULL);\n");
    fprintf(execute_src_file, "    pthread_join(publish_thread, NULL);\n");
    fprintf(execute_src_file, "    pthread_join(input_thread, NULL);\n");
    fprintf(execute_src_file, "    return 0;\n");
    fprintf(execute_src_file, "}\n\n");


    /* 提出ファイルの読み込み */
    const char *filename = "./data/test/test1.docx";                                              // 提出ファイルのパス
    char cmd[256];                                                                                // docxをテキストに変換するコマンド
    snprintf(cmd, sizeof(cmd), "python3 ./scripts/convert_docx2txt.py --input \"%s\"", filename); // コマンドの生成

    /* docx → txt */
    FILE *fp = popen(cmd, "r");

    if (!fp) {
        perror("popen");
        return 1;
    }

    /* ユーザプログラムの追記 */
    char buf[1024];
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        fprintf(execute_src_file, "%s", buf);
    }

    pclose(fp);
    fclose(execute_src_file);
    return 0;
}
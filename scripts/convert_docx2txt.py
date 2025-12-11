from docx import Document
import argparse

parser = argparse.ArgumentParser(description='Convert .docx to text based on specific markers.')
# 位置引数を残しつつ、--input オプションでも受け取れるようにする。
parser.add_argument('--input', '-i', dest='input', type=str, help='Path to the input .docx file (optional flag)')
args = parser.parse_args()

# 入力パスを位置引数またはオプションのどちらかから取得
input_path = args.input or args.input_opt
if not input_path:
    parser.error('the following arguments are required: input (positional) or --input/-i')



START_STRING = "【プログラム】"
END_STRING   = "【プログラムの説明】"

document = Document(input_path)

capture = False
for p in document.paragraphs:
    if p.text.startswith(START_STRING):
        capture = True
        continue
    if p.text.startswith(END_STRING):
        capture = False
    if not capture:
        continue
    print(p.text)

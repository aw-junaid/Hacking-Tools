from tempfile import NamedTemporaryFile

def write_results(results):
    filename = NamedTemporaryFile(delete=False)
    print(filename.name)
    filename.write(bytes(results,"utf-8"))
    print("Results written to", filename)

write_results("writing in a temp file")

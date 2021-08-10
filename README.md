# log_rotate

Write the standard input data to the file

## Usage example

```bash
$ your_program | log_rotate --file_path /tmp/example/log.txt --file_size=100000 --file_number=3
```

This will output the output of your program to `/tmp/example/log.txt` through `log_rotate`, and limit the size and number of a single file according to the options.

Eventually there will be `log.3.txt, log.2.txt, log.1.txt, log.txt` in the `/tmp/example` directory.

These files will circulate in the order `log.(N).txt -> log.(N+1).txt`, and always keep at least 3 complete files.

The `log.txt` can be viewed in real time using `tail -f /tmp/example/log.txt`.

## Options

```bash
$ ./log_rotate --help                                                      
Usage: ./log_rotate [OPTIONS]

Options:
  -h,--help                             Print this help message and exit
  --file_path TEXT REQUIRED             File path to record log
  --file_size INT REQUIRED              Size of each file
  --file_number INT REQUIRED            Maximum number of files
  --fifo_size INT=32768                 Fifo size
  --stdout                              Should print to stdout
```

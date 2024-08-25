FROM python:2.7-alpine

RUN apk add --update git
RUN git clone https://github.com/lightos/credmap.git
WORKDIR credmap
ENTRYPOINT ["python", "credmap.py"]
CMD ["--help"]

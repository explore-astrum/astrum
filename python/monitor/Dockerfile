FROM python:3.5.5

WORKDIR /app
ADD . .
RUN rm -rf neo-python
RUN git clone https://github.com/CityOfZion/neo-python.git
RUN pip install -r requirements.txt

CMD python monitor.py

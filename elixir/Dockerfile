# Elixir
FROM elixir:1.8.1-alpine
ENV MIX_ENV=prod
ENV REPLACE_OS_VARS=true
RUN apk add --no-cache alpine-sdk
RUN apk add --no-cache bash
RUN apk add --no-cache git

RUN yes | mix local.hex
RUN yes | mix local.rebar

WORKDIR /tmp
ADD mix.exs .
ADD mix.lock .
RUN mix deps.get
RUN mix deps.compile
RUN ls

WORKDIR /app
ADD . .
RUN ls -lah
RUN rm -rf deps
RUN rm -rf _build
RUN mv /tmp/deps .
RUN mv /tmp/_build .

# RUN mix test
RUN mix release
RUN chmod +x "_build/prod/rel/astrum/bin/astrum"

FROM elixir:1.8.1-alpine
RUN apk add --no-cache bash
WORKDIR /app
COPY --from=0 /app/_build/prod/rel/astrum /app
RUN find .

CMD ["bin/astrum",  "foreground"]

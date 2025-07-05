FROM gcc:14

ARG UID=1000
ARG GID=10

RUN apt-get update && \
apt-get install -y --no-install-recommends linux-libc-dev && \
rm -rf /var/lib/apt/lists/*00

# Create the group only if that numeric GID is not present.
RUN getent group $GID >/dev/null || groupadd -g $GID dev \
 && useradd -m -u $UID -g $GID -s /bin/bash dev

USER dev
WORKDIR /workspace

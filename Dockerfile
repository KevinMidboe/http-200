# Use the scratch base image
FROM scratch

WORKDIR /app
COPY server .

# Expose port 80
EXPOSE 80

# Run the binary
CMD ["/app/server"]

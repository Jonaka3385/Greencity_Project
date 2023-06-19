function decodeUplink(input) {
    return {
      data: {
        bodenfeuchtigkeit: input.bytes
      },
      warnings: [],
      errors: []
    };
  }
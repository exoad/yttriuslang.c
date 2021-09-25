const content = require("../../../configs/token.json");
var compiler = require("compilex");

module.exports = {
  config: {
    name: `flush`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message, args) => {
    if (content.owner_id == message.author.id) {
      try {
        compiler.flush(function () {
          console.log("Flushed all runners!");
          message.channel.send("**Flushed all files!**");
        });
      } catch (e) {
        console.log(e);
      }
    } else
      message.channel.send(
        "**" +
          message.author.username +
          "** You do not have permission to use this command! Only the owner can."
      );
  },
};

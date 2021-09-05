const JSONdb = require("simple-json-db");
const db = new JSONdb("db/registry_user.json");
module.exports = {
  config: {
    name: `register`,
    category: "",
    description: "",
    aliases: [`adduser`],
  },
  run: async (bot, message, args) => {
    try {
      let author = message.author.id;
      message.channel.send("Checking the database...").then((m) => {
        setTimeout(function () {
          if (!db.has(author)) {
            db.set(author, "1");
            m.edit(
              "**Registered " +
                message.author.username +
                " to the database successfully.**"
            );
          } else {
            m.edit("**Failed to register you.**\nReason: You are already a registered user!");
          }
        }, 1000);
      });
    } catch (err) {
      const embed = new MessageEmbed()
        .setTitle("Whoops, looks like something went wrong!")
        .setThumbnail(resource.aw_snap)
        .setDescription(
          "Use `" +
            config.prefix +
            "help` for a list of avaliable commands or use `" +
            config.prefix +
            "support` to join the support server!"
        )
        .setFooter("Still facing issues? Join the support server!");
      message.channel.send({ embed }).then((m) => {
        m.delete({ timeout: 5000 });
      });
    }
  },
};

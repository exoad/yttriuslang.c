const { MessageEmbed, DiscordAPIError } = require("discord.js");
const Discord = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
const superagent = require("superagent");
const colors = require("../../configs/colors.json");
const token = require("../../configs/token.json");
module.exports = {
  config: {
    name: `docs`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message, args) => {
    try {
      //function only accepts a value of .md or .pdf or .html etc.
      function askFile(filetype) {
        message.channel
          .send("Fetching documentations for type `"+filetype.replace('.', "").toUpperCase()+"`...")
          .then((m) => {
            setTimeout(function () {
              m.edit(
                "**Fetched it successfully!**\nOther formats are: `md, txt, html, pdf`"
              );
              const doc = new Discord.MessageAttachment(
                "lib/archives/README."+filetype
              );
              message.channel.send(doc);
            }, 1000);
          });
      }
      let file_type = args[0];
      if (!file_type || file_type == undefined || file_type == null) {
        const embed = new MessageEmbed()
          .setTitle(
            "Oops, this command requires some input arguments to proceed!"
          )
          .setDescription(
            "This command requires arguments after the inital command in order to give you the correct result!"
          )
          .addField(
            "Command Usage",
            "```" + token.prefix + "docs [file_type]```"
          )
          .addField(
            "file_type",
            "**Description:** The file type specified to view the documentations\n**Avaliable Parameters:** `md, markdown, txt, text, html, pdf`"
          )
          .addField(
            "Command Description",
            "Gets the documentation for this service."
          )
          .addField(
            "Command Example Usage",
            "```" + token.prefix + "docs md```"
          )
          .setThumbnail(resource.aw_snap)
          .setColor(colors.error);
        message.channel.send({ embed });
      } else if (file_type == "md" || file_type == "markdown") {
        askFile(".md");
      } else if (file_type == "txt" || file_type == "text") {
        askFile(".txt");
      } else if (
        file_type == "html" ||
        file_type == "mhtml" ||
        file_type == "htm"
      ) {
        askFile(".html");
      } else if (file_type == "pdf") {
        askFile(".pdf");
      } else {
        const embed = new MessageEmbed()
          .setTitle(
            "Oops, this command requires some input arguments to proceed!"
          )
          .setDescription(
            "This command requires arguments after the inital command in order to give you the correct result!"
          )
          .addField(
            "Command Usage",
            "```" + token.prefix + "docs [file_type]```"
          )
          .addField(
            "file_type",
            "**Description:** The file type specified to view the documentations\n**Avaliable Parameters:** `md, markdown, txt, text, html, pdf`"
          )
          .addField(
            "Command Description",
            "Gets the documentation for this service."
          )
          .addField(
            "Command Example Usage",
            "```" + token.prefix + "docs md```"
          )
          .setThumbnail(resource.aw_snap)
          .setColor(colors.error);
        message.channel.send({ embed });
      }
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

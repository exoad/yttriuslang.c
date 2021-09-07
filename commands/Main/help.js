/*
 * TODO: Make all science commands a sub category
 */
const { MessageEmbed } = require("discord.js");
const config = require("../../configs/token.json");
const chnl = require("../../configs/chnl.json");
const resource = require("../../configs/resource.json");
module.exports = {
  config: {
    name: `help`,
    category: "",
    description: "",
    aliases: [``],
  },
  run: async (bot, message, args) => {
    try {
      let cat = args[0];
      let cat_cmd = args[1];
      //this function makes the embeds so there wont be repetitive embeds everywhere
      function makeCommandList(menu_name, list_items, isCat) {
        if (isCat) {
          const embed = new MessageEmbed()
            .setTitle(menu_name + " Category")
            .setDescription(
              "Here are all the avaliable " +
                menu_name.toLowerCase() +
                " sub-categories.\nUse `" +
                config.prefix +
                "help " +
                menu_name.toLowerCase() +
                " {subcategory}`"
            )
            .addField("Categories", "```" + list_items + "```")
            .setThumbnail(resource.yet);

          message.channel.send({ embed });
        } else {
          const embed = new MessageEmbed()
            .setTitle(menu_name + " Commands")
            .setDescription(
              "Here are all commands for the category " +
                menu_name.toLowerCase() +
                ".\nUse `" +
                config.prefix +
                "cmd {command}`"
            )
            .addField("Commands", "```" + list_items + "```")
            .setThumbnail(resource.yet);

          message.channel.send({ embed });
        }
      }
      if (!cat || cat == undefined) {
        const embed = new MessageEmbed() // <-- Main Entry Point
          .setTitle("Main Help Menu | Categories & Sub-Categories")
          .setDescription(
            "Here you will find all of the categories! Wanting to find info on a specific command? Use `" +
              config.prefix +
              "cmd {command}`Need support? Join my discord server [here](" +
              chnl.spprt_server +
              ")"
          )
          .addField(
            ":jigsaw: General Commands",
            `Use \`${config.prefix}help general\` to access all generalized / utility / fun commands`,
            true
          )
          .addField(
            ":test_tube: Science Categories",
            `Use  \`${config.prefix}help science\` to access all sub-categories for science`,
            true
          )
          .addField(
            ":tools: Tools Categories",
            `Use  \`${config.prefix}help tools\` to access commands that are general tools (calculators, etc.)`,
            true
          )
          .addField(
            ":jigsaw: Other Categories",
            `Use  \`${config.prefix}help others\` to access all other sub-categories that could not fit onto this list`,
            true
          )
          .addField(
            ":dividers: Social Commands",
            `Use \`${config.prefix}help social\` to access all Account related commands`
          )
          .setThumbnail(resource.science)
          .setFooter('The prefix is "' + config.prefix + '"!');

        message.channel.send({ embed });
      } else {
        if (
          (cat == "science" || cat == "sci") &&
          (!cat_cmd || cat_cmd == undefined)
        )
          makeCommandList(
            "Science",
            "chemistry, astronomy, otherscience",
            true
          );
        else if (
          (cat_cmd == "chemistry" || cat_cmd == "chem") &&
          (cat == "science" || cat == "sci")
        )
          makeCommandList(
            "Chemistry",
            "periodic, element, molar, elehistory",
            false
          );
        else if (
          (cat_cmd == "astro" || cat_cmd == "astronomy") &&
          (cat == "science" || cat == "sci")
        )
          makeCommandList(
            "Astronomy",
            "iss, apod, earth, moonphase, calcsun",
            false
          );
        else if (
          (cat_cmd == "otherscience" || cat_cmd == "othersci") &&
          (cat == "science" || cat == "sci")
        )
          makeCommandList("Other Science", "earth", false);
        else if (cat == "general")
          makeCommandList(
            "General",
            "help, support, cmd, invite, docs, botinfo, guide",
            false
          );
        else if (cat == "tools" || cat == "tool")
          makeCommandList("Tools", "math, whatis, tex", false);
        else if (
          (cat == "others" || cat == "other") &&
          (!cat_cmd || cat_cmd == undefined)
        )
          makeCommandList("Other", "fun", true);
        else if(cat == "social")
          makeCommandList("Social", "myaccount, userdocs, register", false);
      }
    } catch (err) {
      const embed = new MessageEmbed()
        .setTitle(
          "Whoops, looks like the requested command doesn't exist (yet)!"
        )
        .setThumbnail(resource.aw_snap)
        .setDescription(
          "Use `" + config.prefix + "help` for a list of avaliable commands!"
        )
        .setFooter("Still facing issues? Join the support server!");
      message.channel.send({ embed }).then((m) => {
        m.delete({ timeout: 5000 });
      });
    }
  },
};

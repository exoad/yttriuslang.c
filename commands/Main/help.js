const { MessageEmbed } = require('discord.js');
const config = require('../../configs/config.json');
const chnl = require('../../configs/chnl.json');
const resource = require('../../configs/resource.json');
module.exports = {
    config: {
        name: `help`,
        category: '',
        description: '',
        aliases: [``]
    },
    run: async (bot, message, args) => {
      try {
        if(args[0] == "general") {
            const embed = new MessageEmbed() // <-- Entry point for all Generalized Commands
            .setTitle("General Commands")
            .setDescription("Here are all the commands related to the regular functions of the bot. To find info on a specific command use `"+config.prefix+"cmd {command}`")
            .addField("Commands List", "```help, support, cmd, invite```")
            .setThumbnail(resource.yet)
            .setAuthor(message.author.username, message.author.displayAvatarURL)

            message.channel.send({embed})
        } else if(args[0] == "astro" || args[0] == "astronomy") {
            const embed = new MessageEmbed() // <-- Entry point for all Astronomical Commands
            .setTitle("Astronomy Commands")
            .setDescription("Here are all the commands related to Astronomy. To find info on a specific command use `"+config.prefix+"cmd {command}`")
            .addField("Commands List", "```iss, apod, earth, moonphase, calcsun```")
            .setThumbnail(resource.astro)
            .setAuthor(message.author.username, message.author.displayAvatarURL)

            message.channel.send({embed})
        } else if(args[0] == "chem" || args[0] == "chemistry") {
            const embed = new MessageEmbed() // <-- Entry point for all Chemistry commands
            .setTitle("Chemistry Commands")
            .setDescription("Here are all the commands related to Chemistry. To find info on a specific command use `"+config.prefix+"cmd {command}`")
            .addField("Commands List", "```periodic, element, molar, elehistory```")
            .setAuthor(message.author.username, message.author.displayAvatarURL)
            .setThumbnail(resource.chem)

            message.channel.send({embed})
        } else if(args[0] == "tools" || args[0] == "tool") {
            const embed = new MessageEmbed() // <-- Entry point for all other commands
            .setTitle("Tool Commands")
            .setDescription("Here are all the commands that are valid tools that can be used (calculators, etc.). To find info on a specific command use `"+config.prefix+"cmd {command}`")
            .addField("Commands List", "```math, whatis, tex```")
            .setThumbnail(resource.yet)
            .setAuthor(message.author.username, message.author.displayAvatarURL)

            message.channel.send({embed})
        } else if(args[0] == "sci" || args[0] == "science") {
            const embed = new MessageEmbed()
            .setTitle("Other Science Commands")
            .setDescription("Here are all the commands related to other science topics/fields. To find info on a specific command use `"+config.prefix+"cmd {command}`")
            .addField("Earth Science", "```earth```")
            .setThumbnail(resource.yet)
            .setAuthor(message.author.username, message.author.displayAvatarURL)

            message.channel.send({embed})
        } else if(args[0] == "others" || args[0] == "other") {
            const embed = new MessageEmbed()
            .setTitle("Other Commands")
            .setDescription("Here are all the commands that cannot fit into a definitive category. To find info on a specific command use `"+config.prefix+"cmd {command}`")
            .addField("Commands List", "```fact```")
            .setThumbnail(resource.yet)
            .setAuthor(message.author.username, message.author.displayAvatarURL)

            message.channel.send({embed})
        } else {
            const embed = new MessageEmbed() // <-- Main Entry Point
            .setTitle("Main Help Menu")
            .setDescription("Here you will find all of the categories! Wanting to find info on a specific command? Use `"+config.prefix+"cmd {command}`Need support? Join my discord server [here]("+chnl.spprt_server+")")
            .addField(":jigsaw: General", `Use \`${config.prefix}help general\` to access all generalized / utility / fun commands`, true)
            .addField(":milky_way: Astronomy", `Use  \`${config.prefix}help astro\` to access all astronomy related commands`, true)
            .addField(":test_tube: Chemistry", `Use  \`${config.prefix}help chem\` to access all chemistry related commands`, true)
            .addField(":tools: Tools", `Use  \`${config.prefix}help tools\` to access commands that are general tools (calculators, etc.)`, true)
            .addField(":jigsaw: Others", `Use  \`${config.prefix}help others\` to access commands that cannot fit into any categories`, true)
            .setThumbnail(resource.science)
            .setFooter("The prefix is \""+config.prefix+"\"!")
    
            message.channel.send({embed})
        }
    } catch (err) {
        const embed = new MessageEmbed()
        .setTitle("Whoops, looks like the requested command doesn't exist (yet)!")
        .setThumbnail(resource.aw_snap)
        .setDescription("Use `"+config.prefix+"help` for a list of avaliable commands!")
        .setFooter("Still facing issues? Join the support server!")
          message.channel.send({ embed }).then(m => {
              m.delete({ timeout: 5000 })
          });
    }
  }
  }